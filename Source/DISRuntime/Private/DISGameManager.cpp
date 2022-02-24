// Fill out your copyright notice in the Description page of Project Settings.

#include "DISGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "DISComponent.h"
#include "PDUProcessor.h"

DEFINE_LOG_CATEGORY(LogDISGameManager);

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
			UE_LOG(LogDISGameManager, Error, TEXT("Multiple DISGameManager actors found. Only one actor should be used to configure your  DIS Game Manager."));
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
			RawDISClassMappings.insert({ EntityType, DISMapping.DISEntity });
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

void ADISGameManager::HandleEntityStatePDU(UGRILL_EntityStatePDU* EntityStatePDUIn)
{
	if (!IsValid(EntityStatePDUIn))
	{
		UE_LOG(LogDISGameManager, Warning, TEXT("Received Entity State PDU was not valid. Ignoring the PDU."));
		return;
	}

	//Find associated actor in the DISActorMappings map -- If actor does not exist spawn one
	AActor* associatedActor = RawDISActorMappings[EntityStatePDUIn->EntityStatePDUStruct.EntityID];
	if (associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		UDISComponent* DISComponent = IDISInterface::Execute_GetActorDISComponent(associatedActor);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleEntityStatePDU(EntityStatePDUIn);
		}
	}
	else
	{
		//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
		if (EntityStatePDUIn->EntityStatePDUStruct.EntityAppearance & (1 << 23))
		{
			UE_LOG(LogDISGameManager, Log, TEXT("Received Entity State PDU with a Deactivated Entity Appearance for an entity that is not in the level. Ignoring the PDU. Entity marking: %s"), *EntityStatePDUIn->EntityStatePDUStruct.Marking);
			return;
		}

		SpawnNewEntityFromEntityState(EntityStatePDUIn);
	}
}

void ADISGameManager::HandleEntityStateUpdatePDU(UGRILL_EntityStateUpdatePDU* EntityStateUpdatePDUIn)
{
	if (!IsValid(EntityStateUpdatePDUIn))
	{
		UE_LOG(LogDISGameManager, Warning, TEXT("Received Entity State Update PDU was not valid. Ignoring the PDU."));
		return;
	}

	//Find associated actor in the DISActorMappings map -- If actor does not exist spawn one
	AActor* associatedActor = RawDISActorMappings[EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityID];
	if (associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		UDISComponent* DISComponent = IDISInterface::Execute_GetActorDISComponent(associatedActor);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleEntityStateUpdatePDU(EntityStateUpdatePDUIn);
		}
	}
	else
	{
		//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
		if (EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityAppearance & (1 << 23))
		{
			UE_LOG(LogDISGameManager, Log, TEXT("Received Entity State Update PDU with a Deactivated Entity Appearance for an entity that is not in the level. Ignoring the PDU. Entity ID: %s"), *EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityID.ToString());
			return;
		}

		UGRILL_EntityStatePDU* espdu = NewObject<UGRILL_EntityStatePDU>();
		espdu->SetupFromEntityStateUpdatePDU(EntityStateUpdatePDUIn);

		SpawnNewEntityFromEntityState(espdu);
	}
}

void ADISGameManager::HandleFirePDU(UGRILL_FirePDU* FirePDUIn)
{
	if (!IsValid(FirePDUIn))
	{
		UE_LOG(LogDISGameManager, Warning, TEXT("Received Fire PDU was not valid. Ignoring the PDU."));
		return;
	}

	//Get associated OpenDISComponent and relay information
	UDISComponent* DISComponent = GetAssociatedDISComponent(FirePDUIn->WarfareFamilyPDUStruct.FiringEntityID);

	if (DISComponent != nullptr)
	{
		DISComponent->HandleFirePDU(FirePDUIn);
	}
}

void ADISGameManager::HandleDetonationPDU(UGRILL_DetonationPDU* DetonationPDUIn)
{
	if (!IsValid(DetonationPDUIn)) 
	{
		UE_LOG(LogDISGameManager, Warning, TEXT("Received Detonation PDU was not valid. Ignoring the PDU."));
		return;
	}

	//Get associated OpenDISComponent and relay information
	UDISComponent* DISComponent = GetAssociatedDISComponent(DetonationPDUIn->DetonationPDUStruct.MunitionEntityID);

	if (DISComponent != nullptr)
	{
		DISComponent->HandleDetonationPDU(DetonationPDUIn);
	}
}

void ADISGameManager::HandleRemoveEntityPDU(UGRILL_RemoveEntityPDU* RemoveEntityPDUIn)
{
	if (!IsValid(RemoveEntityPDUIn))
	{
		UE_LOG(LogDISGameManager, Warning, TEXT("Received Remove Entity PDU was not valid. Ignoring the PDU."));
		return;
	}

	//Verify that we are the appropriate sim to handle the RemoveEntityPDU
	if (RemoveEntityPDUIn->SimManagementFamilyPDUStruct.ReceivingEntityID.Site == SiteID && RemoveEntityPDUIn->SimManagementFamilyPDUStruct.ReceivingEntityID.Application == ApplicationID)
	{
		//Get associated OpenDISComponent and relay information
		UDISComponent* DISComponent = GetAssociatedDISComponent(RemoveEntityPDUIn->SimManagementFamilyPDUStruct.ReceivingEntityID);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleRemoveEntityPDU(RemoveEntityPDUIn);
		}
	}
}

void ADISGameManager::SpawnNewEntityFromEntityState(UGRILL_EntityStatePDU* EntityStatePDUIn)
{
	if (!IsValid(EntityStatePDUIn))
	{
		UE_LOG(LogDISGameManager, Warning, TEXT("Entity State PDU that was passed to spawn an entity from was not valid. Ignoring the PDU."));
		return;
	}

	//If an actor was not found -- check to see if there is an associated actor for the entity type
	TSoftClassPtr<AActor>* associatedSoftClassReference = &RawDISClassMappings[EntityStatePDUIn->EntityStatePDUStruct.EntityType];
	UClass* associatedClass = associatedSoftClassReference->LoadSynchronous();
	if (!associatedClass)
	{
		std::map<FEntityType, TSoftClassPtr<AActor>> WildcardMappings;
		for (auto Pair : RawDISClassMappings)
		{
			FEntityType Key = Pair.first;
			FEntityType FilledKey = FEntityType(Pair.first).FillWildcards(EntityStatePDUIn->EntityStatePDUStruct.EntityType);
			if (!(Key == FilledKey))
			{
				Key = FilledKey;
				//WildcardMappings.Add(Key, Pair.Value);
				WildcardMappings.insert({ Key, Pair.second });
			}
		}
		TSoftClassPtr<AActor>* NewSoftClassRef = &WildcardMappings[EntityStatePDUIn->EntityStatePDUStruct.EntityType];
		if (NewSoftClassRef != nullptr) {
			associatedClass = NewSoftClassRef->LoadSynchronous();
		}
	}
	//If so, spawn one and relay information to the associated component
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
			AddDISEntityToMap(EntityStatePDUIn->EntityStatePDUStruct.EntityID, spawnedActor);

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
		UE_LOG(LogDISGameManager, Warning, TEXT("No mapping exists between an actor and the DIS enumeration of: %s"), *EntityStatePDUIn->EntityStatePDUStruct.EntityType.ToString());
	}
}

UDISComponent* ADISGameManager::GetAssociatedDISComponent(FEntityID EntityIDIn)
{
	UDISComponent* DISComponent = nullptr;

	//Find associated actor in the DISActorMappings map
	AActor* associatedActor = RawDISActorMappings[EntityIDIn];
	if (associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		DISComponent = IDISInterface::Execute_GetActorDISComponent(associatedActor);
	}

	return DISComponent;
}

void ADISGameManager::AddDISEntityToMap(FEntityID EntityIDToAdd, AActor* EntityToAdd)
{
	DISActorMappings.Add(EntityIDToAdd, EntityToAdd);
	RawDISActorMappings[EntityIDToAdd] = EntityToAdd;
}

bool ADISGameManager::RemoveDISEntityFromMap(FEntityID EntityIDToRemove)
{
	DISActorMappings.Remove(EntityIDToRemove);
	const int AmountRemoved = RawDISActorMappings.erase(EntityIDToRemove);
	return (AmountRemoved > 0);
}