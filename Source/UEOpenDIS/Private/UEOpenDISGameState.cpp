// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOpenDISGameState.h"
#include "UEOpenDISRuntimeSettings.h"
#include "OpenDISComponent.h"
#include "DISEntity_Base.h"
#include "PDUProcessor.h"

DEFINE_LOG_CATEGORY(LogOpenDIS);

AUEOpenDISGameState::AUEOpenDISGameState()
{
	//Load settings from the OpenDIS ProjectSettings
	const UUEOpenDISRuntimeSettings* const Settings = UUEOpenDISRuntimeSettings::Get();
	check(Settings);
	//Initialize DISClassMappings from the loaded settings
	for (FOpenDISEnumerationMappings DISMapping : Settings->DISClassMappings)
	{
		for (FEntityType EntityType : DISMapping.AssociatedDISEnumerations)
		{
			DISClassMappings.Add(EntityType, DISMapping.DISEntity);
		}
	}

	ExerciseID = Settings->ExerciseID;
	SiteID = Settings->SiteID;
	ApplicationID = Settings->ApplicationID;
}

// Called when the game starts
void AUEOpenDISGameState::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnEntityStatePDUProcessed.AddDynamic(this, &AUEOpenDISGameState::HandleEntityStatePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnEntityStateUpdatePDUProcessed.AddDynamic(this, &AUEOpenDISGameState::HandleEntityStateUpdatePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnFirePDUProcessed.AddDynamic(this, &AUEOpenDISGameState::HandleFirePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnDetonationPDUProcessed.AddDynamic(this, &AUEOpenDISGameState::HandleDetonationPDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnRemoveEntityPDUProcessed.AddDynamic(this, &AUEOpenDISGameState::HandleRemoveEntityPDU);
}

void AUEOpenDISGameState::HandleOnDISEntityDestroyed(AActor* DestroyedActor)
{
	bool anyRemoved = false;

	//Remove the actor from the dis entity mapping
	UOpenDISComponent* DISComponent = DestroyedActor->FindComponentByClass<UOpenDISComponent>();

	if (DISComponent != nullptr)
	{
		anyRemoved = RemoveDISEntityFromMap(DISComponent->EntityID);
	}

	if (!anyRemoved)
	{
		UE_LOG(LogOpenDIS, Error, TEXT("Failed to remove %s from the Entity Map!"), *DestroyedActor->GetName());
	}
}

void AUEOpenDISGameState::HandleEntityStatePDU(FEntityStatePDU EntityStatePDUIn)
{
	//Find associated actor in the DISActorMappings map -- If actor does not exist spawn one
	ADISEntity_Base** associatedActor = DISActorMappings.Find(EntityStatePDUIn.EntityID);
	if (associatedActor != nullptr && *associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
		UOpenDISComponent* DISComponent = (*associatedActor)->FindComponentByClass<UOpenDISComponent>();

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
			UE_LOG(LogOpenDIS, Log, TEXT("Received Entity State PDU with a Deactivated Entity Appearance for an entity that is not in the level. Ignoring the PDU. Entity marking: %s"), *EntityStatePDUIn.Marking);
			return;
		}

		SpawnNewEntityFromEntityState(EntityStatePDUIn);
	}
}

void AUEOpenDISGameState::HandleEntityStateUpdatePDU(FEntityStateUpdatePDU EntityStateUpdatePDUIn)
{
	//Find associated actor in the DISActorMappings map -- If actor does not exist spawn one
	ADISEntity_Base** associatedActor = DISActorMappings.Find(EntityStateUpdatePDUIn.EntityID);
	if (associatedActor != nullptr && *associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
		UOpenDISComponent* DISComponent = (*associatedActor)->FindComponentByClass<UOpenDISComponent>();

		if (DISComponent != nullptr)
		{
			DISComponent->HandleEntityStateUpdatePDU(EntityStateUpdatePDUIn);
		}
	}
	else
	{
		//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
		if (EntityStateUpdatePDUIn.EntityAppearance & (1 << 23))
		{
			UE_LOG(LogOpenDIS, Log, TEXT("Received Entity State Update PDU with a Deactivated Entity Appearance for an entity that is not in the level. Ignoring the PDU. Entity ID: %s"), *EntityStateUpdatePDUIn.EntityID.ToString());
			return;
		}

		SpawnNewEntityFromEntityState(EntityStateUpdatePDUIn);
	}
}

void AUEOpenDISGameState::HandleFirePDU(FFirePDU FirePDUIn)
{
	//Get associated OpenDISComponent and relay information
	UOpenDISComponent* DISComponent = GetAssociatedOpenDISComponent(FirePDUIn.MunitionEntityID);

	if (DISComponent != nullptr)
	{
		DISComponent->HandleFirePDU(FirePDUIn);
	}
}

void AUEOpenDISGameState::HandleDetonationPDU(FDetonationPDU DetonationPDUIn)
{
	//Get associated OpenDISComponent and relay information
	UOpenDISComponent* DISComponent = GetAssociatedOpenDISComponent(DetonationPDUIn.MunitionEntityID);

	if (DISComponent != nullptr)
	{
		DISComponent->HandleDetonationPDU(DetonationPDUIn);
	}
}

void AUEOpenDISGameState::HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn)
{
	//Verify that we are the appropriate sim to handle the RemoveEntityPDU
	if (RemoveEntityPDUIn.ReceivingEntityID.Site == SiteID && RemoveEntityPDUIn.ReceivingEntityID.Application == ApplicationID)
	{
		//Get associated OpenDISComponent and relay information
		UOpenDISComponent* DISComponent = GetAssociatedOpenDISComponent(RemoveEntityPDUIn.ReceivingEntityID);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleRemoveEntityPDU(RemoveEntityPDUIn);
		}
	}
}

void AUEOpenDISGameState::SpawnNewEntityFromEntityState(FEntityStatePDU EntityStatePDUIn) 
{
	//If an actor was not found -- check to see if there is an associated actor for the entity type
	TAssetSubclassOf<ADISEntity_Base>* associatedSoftClassReference = DISClassMappings.Find(EntityStatePDUIn.EntityType);
	if (associatedSoftClassReference != nullptr)
	{
		//If so, spawn one and relay information to the associated component
		UClass* associatedClass = associatedSoftClassReference->LoadSynchronous();
		if (associatedClass != nullptr)
		{ 
			ADISEntity_Base* spawnedActor = GetWorld()->SpawnActor<ADISEntity_Base>(associatedClass);

			if (spawnedActor != nullptr)
			{
				spawnedActor->OnDestroyed.AddDynamic(this, &AUEOpenDISGameState::HandleOnDISEntityDestroyed);

				//Get DIS Component of the newly spawned actor
				//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
				UOpenDISComponent* DISComponent = spawnedActor->FindComponentByClass<UOpenDISComponent>();
				//Add actor to the map
				AddDISEntityToMap(EntityStatePDUIn.EntityID, spawnedActor);

				if (DISComponent != nullptr)
				{
					DISComponent->SpawnedFromNetwork = true;
					DISComponent->HandleEntityStatePDU(EntityStatePDUIn);
				}
			}
		}
	}
	else
	{
		//Otherwise notify the user that no such mapping exists
		UE_LOG(LogOpenDIS, Warning, TEXT("No mapping exists between an actor and the DIS enumeration of: %s"), *EntityStatePDUIn.EntityType.ToString());
	}
}

UOpenDISComponent* AUEOpenDISGameState::GetAssociatedOpenDISComponent(FEntityID EntityIDIn)
{
	UOpenDISComponent* DISComponent = nullptr;

	//Find associated actor in the DISActorMappings map
	ADISEntity_Base** associatedActor = DISActorMappings.Find(EntityIDIn);
	if (associatedActor != nullptr && *associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
		DISComponent = (*associatedActor)->FindComponentByClass<UOpenDISComponent>();
	}

	return DISComponent;
}

void AUEOpenDISGameState::AddDISEntityToMap(FEntityID EntityIDToAdd, ADISEntity_Base* EntityToAdd)
{
	DISActorMappings.Add(EntityIDToAdd, EntityToAdd);
}

bool AUEOpenDISGameState::RemoveDISEntityFromMap(FEntityID EntityIDToRemove)
{
	int amountRemoved = DISActorMappings.Remove(EntityIDToRemove);

	return (amountRemoved > 0);
}