// Fill out your copyright notice in the Description page of Project Settings.

#include "DISGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "DISComponent.h"
#include "PDUProcessor.h"
#include "UDPSubsystem.h"

DEFINE_LOG_CATEGORY(LogDISGameManager);

ADISGameManager::ADISGameManager() 
{
	PrimaryActorTick.bCanEverTick = true;
}

ADISGameManager* ADISGameManager::GetDISGameManager(UObject* WorldContextObject)
{
	ADISGameManager* Actor = nullptr;

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(World, ADISGameManager::StaticClass(), Actors);
		int NbActors = Actors.Num();
		if (NbActors == 0)
		{
			UE_LOG(LogDISGameManager, Error, TEXT("DISGameManager actor not found. Please add one to your world to configure your DIS Game Manager."));
		}
		else if (NbActors > 1)
		{
			UE_LOG(LogDISGameManager, Error, TEXT("Multiple DISGameManager actors found. Only one actor should be used to configure your DIS Game Manager."));
		}
		else
		{
			Actor = Cast<ADISGameManager>(Actors[0]);
		}
	}

	return Actor;
}

// Called when the game starts
void ADISGameManager::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnEntityStatePDUProcessed.AddDynamic(this, &ADISGameManager::HandleEntityStatePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnEntityStateUpdatePDUProcessed.AddDynamic(this, &ADISGameManager::HandleEntityStateUpdatePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnFirePDUProcessed.AddDynamic(this, &ADISGameManager::HandleFirePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnDetonationPDUProcessed.AddDynamic(this, &ADISGameManager::HandleDetonationPDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnRemoveEntityPDUProcessed.AddDynamic(this, &ADISGameManager::HandleRemoveEntityPDU);

	//Auto connect sockets if needed
	if (AutoConnectReceiveAddresses) 
	{
		for (FSocketInfo socket : ReceiveSocketsToSetup)
		{
			int SocketID;
			GetGameInstance()->GetSubsystem<UUDPSubsystem>()->OpenReceiveSocket(FSocketSettings(), SocketID, socket.IpAddress, socket.Port);
		}
	}
	if (AutoConnectSendAddresses)
	{
		for (FSocketInfo socket : SendSocketsToSetup)
		{
			int SocketID;
			GetGameInstance()->GetSubsystem<UUDPSubsystem>()->OpenSendSocket(FSocketSettings(), SocketID, socket.IpAddress, socket.Port);
		}
	}

	if (DISClassEnum) 
	{
		//Initialize DISClassMappings from the loaded settings
		for (FDISClassEnumStruct DISMapping : DISClassEnum->DISClassEnumArray)
		{
			for (FEntityType EntityType : DISMapping.AssociatedDISEnumerations)
			{
				//If an actor was not found -- check to see if there is an associated actor for the entity type
				TSoftClassPtr<AActor>* associatedSoftClassReference = DISClassMappings.Find(EntityType);

				if (associatedSoftClassReference != nullptr)
				{
					UE_LOG(LogDISGameManager, Warning, TEXT("A DIS Enumeration mapping already exists for %s and is linked to %s. This enumeration will now point to: %s"), *EntityType.ToString(), *associatedSoftClassReference->GetAssetName(), *DISMapping.DISEntity.GetAssetName());
				}

				DISClassMappings.Add(EntityType, DISMapping.DISEntity);
				RawDISClassMappings.insert_or_assign(EntityType, DISMapping.DISEntity);
			}
		}
	}
	else
	{
		UE_LOG(LogDISGameManager, Error, TEXT("No DIS Class Enum Mapping has been set within the DIS Game Manager actor!"));
	}
}

void ADISGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (std::pair<FEntityID, AActor*> DisEntity : RawDISActorMappings)
	{
		if (IsValid(DisEntity.second))
		{
			UDISComponent* DISComponent = IDISInterface::Execute_GetActorDISComponent(DisEntity.second);

			if (DISComponent)
			{
				DISComponent->DoDeadReckoning(DeltaTime);
			}
			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannot find DISComponent on entity %s"), *DisEntity.second->GetName())
			}
		}
		else
		{
			UE_LOG(LogDISGameManager, Error, TEXT("Encountered null reference within RawDISActorMapping! Check C++ side usage of RawDISActorMapping to verify using properly!"));
		}
	}
}

void ADISGameManager::HandleOnDISEntityDestroyed(AActor* DestroyedActor)
{
	bool anyRemoved = false;

	//Remove the actor from the dis entity mapping
	UDISComponent* DISComponent = IDISInterface::Execute_GetActorDISComponent(DestroyedActor);

	if (DISComponent != nullptr)
	{
		anyRemoved = RemoveDISEntityFromMap(DISComponent->EntityID);
	}

	if (!anyRemoved)
	{
		UE_LOG(LogDISGameManager, Error, TEXT("Failed to remove %s from the Entity Map!"), *DestroyedActor->GetName());
	}
}

void ADISGameManager::HandleEntityStatePDU(FEntityStatePDU EntityStatePDUIn)
{
	if (EntityStatePDUIn.ExerciseID == ExerciseID)
	{
		//Find associated actor in the DISActorMappings map -- If actor does not exist spawn one
		auto associatedActor = RawDISActorMappings.find(EntityStatePDUIn.EntityID);
		if (associatedActor != RawDISActorMappings.end())
		{
			//If an actor was found, relay information to the associated component
			UDISComponent* DISComponent = IDISInterface::Execute_GetActorDISComponent(associatedActor->second);

			if (DISComponent != nullptr)
			{
				DISComponent->HandleEntityStatePDU(EntityStatePDUIn);
			}
		}
		else
		{
			//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
			if (EntityStatePDUIn.EntityAppearance & (1 << 23))
			{
				UE_LOG(LogDISGameManager, Log, TEXT("Received Entity State PDU with a Deactivated Entity Appearance for an entity that is not in the level. Ignoring the PDU. Entity marking: %s"), *EntityStatePDUIn.Marking);
				return;
			}

			SpawnNewEntityFromEntityState(EntityStatePDUIn);
		}
	}
}

void ADISGameManager::HandleEntityStateUpdatePDU(FEntityStateUpdatePDU EntityStateUpdatePDUIn)
{
	if (EntityStateUpdatePDUIn.ExerciseID == ExerciseID)
	{
		// NOTE: Entity State Update PDUs do not contain an Entity Type, so we cannot spawn an entity from one

		//Get associated OpenDISComponent and relay information
		UDISComponent* DISComponent = GetAssociatedDISComponent(EntityStateUpdatePDUIn.EntityID);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleEntityStateUpdatePDU(EntityStateUpdatePDUIn);
		}
	}
}

void ADISGameManager::HandleFirePDU(FFirePDU FirePDUIn)
{
	if (FirePDUIn.ExerciseID == ExerciseID)
	{
		//Get associated OpenDISComponent and relay information
		UDISComponent* DISComponent = GetAssociatedDISComponent(FirePDUIn.FiringEntityID);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleFirePDU(FirePDUIn);
		}
	}
}

void ADISGameManager::HandleDetonationPDU(FDetonationPDU DetonationPDUIn)
{	
	if (DetonationPDUIn.ExerciseID == ExerciseID)
	{
		//Get associated OpenDISComponent and relay information
		UDISComponent* DISComponent = GetAssociatedDISComponent(DetonationPDUIn.MunitionEntityID);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleDetonationPDU(DetonationPDUIn);
		}
	}
}

void ADISGameManager::HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn)
{
	//Verify that we are the appropriate sim to handle the RemoveEntityPDU
	if (RemoveEntityPDUIn.ExerciseID == ExerciseID && RemoveEntityPDUIn.ReceivingEntityID.Site == SiteID && RemoveEntityPDUIn.ReceivingEntityID.Application == ApplicationID)
	{
		//Get associated OpenDISComponent and relay information
		UDISComponent* DISComponent = GetAssociatedDISComponent(RemoveEntityPDUIn.ReceivingEntityID);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleRemoveEntityPDU(RemoveEntityPDUIn);
		}
	}
}

void ADISGameManager::SpawnNewEntityFromEntityState(FEntityStatePDU EntityStatePDUIn)
{	
	auto associatedSoftClassReference = RawDISClassMappings.find(EntityStatePDUIn.EntityType);
	UClass* associatedClass = nullptr;

	//If an actor was not found, check to see if there is a wildcard mapping -- else, load the found actor
	if (associatedSoftClassReference == RawDISClassMappings.end())
	{
		std::map<FEntityType, TSoftClassPtr<AActor>> WildcardMappings;
		for (auto Pair : RawDISClassMappings)
		{
			FEntityType Key = Pair.first;
			FEntityType FilledKey = FEntityType(Pair.first).FillWildcards(EntityStatePDUIn.EntityType);
			if (!(Key == FilledKey))
			{
				Key = FilledKey;
				//WildcardMappings.Add(Key, Pair.Value);
				WildcardMappings.insert({ Key, Pair.second });
			}
		}

		auto NewSoftClassRef = WildcardMappings.find(EntityStatePDUIn.EntityType);
		if (NewSoftClassRef != WildcardMappings.end()) 
		{
			associatedClass = NewSoftClassRef->second.LoadSynchronous();
		}
	}
	else
	{
		associatedClass = associatedSoftClassReference->second.LoadSynchronous();
	}

	//If an actor has been found, spawn one and relay information to the associated component
	if (associatedClass != nullptr)
	{
		FActorSpawnParameters spawnParams = FActorSpawnParameters();
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(associatedClass, spawnParams);

		if (spawnedActor != nullptr)
		{
			spawnedActor->OnDestroyed.AddDynamic(this, &ADISGameManager::HandleOnDISEntityDestroyed);

			//Get DIS Component of the newly spawned actor
			UDISComponent* DISComponent = IDISInterface::Execute_GetActorDISComponent(spawnedActor);
			//Add actor to the map
			AddDISEntityToMap(EntityStatePDUIn.EntityID, spawnedActor);

			if (DISComponent != nullptr)
			{
				DISComponent->SpawnedFromNetwork = true;
				DISComponent->HandleEntityStatePDU(EntityStatePDUIn);
			}
		}
	}
	else
	{
		//Otherwise notify the user that no such mapping exists
		UE_LOG(LogDISGameManager, Warning, TEXT("No mapping exists between an actor and the DIS enumeration of: %s"), *EntityStatePDUIn.EntityType.ToString());
	}
}

UDISComponent* ADISGameManager::GetAssociatedDISComponent(FEntityID EntityIDIn)
{
	UDISComponent* DISComponent = nullptr;

	//Find associated actor in the DISActorMappings map
	auto associatedActor = RawDISActorMappings.find(EntityIDIn);
	if (associatedActor != RawDISActorMappings.end())
	{
		//If an actor was found, relay information to the associated component
		DISComponent = IDISInterface::Execute_GetActorDISComponent(associatedActor->second);
	}

	return DISComponent;
}

void ADISGameManager::AddDISEntityToMap(FEntityID EntityIDToAdd, AActor* EntityToAdd)
{
	DISActorMappings.Add(EntityIDToAdd, EntityToAdd);
	RawDISActorMappings.insert_or_assign(EntityIDToAdd, EntityToAdd);
}

bool ADISGameManager::RemoveDISEntityFromMap(FEntityID EntityIDToRemove)
{
	DISActorMappings.Remove(EntityIDToRemove);
	const int AmountRemoved = RawDISActorMappings.erase(EntityIDToRemove);
	return (AmountRemoved > 0);
}