// Fill out your copyright notice in the Description page of Project Settings.

#include "DISGameState.h"
#include "DISRuntimeSettings.h"
#include "DISComponent.h"
#include "DISEntity_Base.h"
#include "PDUProcessor.h"

DEFINE_LOG_CATEGORY(LogDISGameState);

ADISGameState::ADISGameState()
{
	//Load settings from the OpenDIS ProjectSettings
	const UDISRuntimeSettings* const Settings = UDISRuntimeSettings::Get();
	check(Settings);
	//Initialize DISClassMappings from the loaded settings
	for (FDISEnumerationMappings DISMapping : Settings->DISClassMappings)
	{
		for (FEntityType EntityType : DISMapping.AssociatedDISEnumerations)
		{
			//If an actor was not found -- check to see if there is an associated actor for the entity type
			TAssetSubclassOf<ADISEntity_Base>* associatedSoftClassReference = DISClassMappings.Find(EntityType);

			if (associatedSoftClassReference != nullptr)
			{
				UE_LOG(LogDISGameState, Warning, TEXT("A DIS Enumeration mapping already exists for %s and is linked to %s. This enumeration will now point to: %s"), *EntityType.ToString(), *associatedSoftClassReference->GetAssetName(), *DISMapping.DISEntity.GetAssetName());
			}

			DISClassMappings.Add(EntityType, DISMapping.DISEntity);
		}
	}

	ExerciseID = Settings->ExerciseID;
	SiteID = Settings->SiteID;
	ApplicationID = Settings->ApplicationID;
}

// Called when the game starts
void ADISGameState::BeginPlay()
{
	Super::BeginPlay();

	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnEntityStatePDUProcessed.AddDynamic(this, &ADISGameState::HandleEntityStatePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnEntityStateUpdatePDUProcessed.AddDynamic(this, &ADISGameState::HandleEntityStateUpdatePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnFirePDUProcessed.AddDynamic(this, &ADISGameState::HandleFirePDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnDetonationPDUProcessed.AddDynamic(this, &ADISGameState::HandleDetonationPDU);
	GetGameInstance()->GetSubsystem<UPDUProcessor>()->OnRemoveEntityPDUProcessed.AddDynamic(this, &ADISGameState::HandleRemoveEntityPDU);
}

void ADISGameState::HandleOnDISEntityDestroyed(AActor* DestroyedActor)
{
	bool anyRemoved = false;

	//Remove the actor from the dis entity mapping
	UDISComponent* DISComponent = DestroyedActor->FindComponentByClass<UDISComponent>();

	if (DISComponent != nullptr)
	{
		anyRemoved = RemoveDISEntityFromMap(DISComponent->EntityID);
	}

	if (!anyRemoved)
	{
		UE_LOG(LogDISGameState, Error, TEXT("Failed to remove %s from the Entity Map!"), *DestroyedActor->GetName());
	}
}

void ADISGameState::HandleEntityStatePDU(UGRILL_EntityStatePDU* EntityStatePDUIn)
{
	//Find associated actor in the DISActorMappings map -- If actor does not exist spawn one
	ADISEntity_Base** associatedActor = DISActorMappings.Find(EntityStatePDUIn->EntityStatePduStruct.EntityID);
	if (associatedActor != nullptr && *associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
		UDISComponent* DISComponent = (*associatedActor)->FindComponentByClass<UDISComponent>();

		if (DISComponent != nullptr)
		{
			DISComponent->HandleEntityStatePDU(EntityStatePDUIn);
		}
	}
	else
	{
		//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
		if (EntityStatePDUIn->EntityStatePduStruct.EntityAppearance & (1 << 23))
		{
			UE_LOG(LogDISGameState, Log, TEXT("Received Entity State PDU with a Deactivated Entity Appearance for an entity that is not in the level. Ignoring the PDU. Entity marking: %s"), *EntityStatePDUIn->EntityStatePduStruct.Marking);
			return;
		}

		SpawnNewEntityFromEntityState(EntityStatePDUIn);
	}
}

void ADISGameState::HandleEntityStateUpdatePDU(UGRILL_EntityStateUpdatePDU* EntityStateUpdatePDUIn)
{
	//Find associated actor in the DISActorMappings map -- If actor does not exist spawn one
	ADISEntity_Base** associatedActor = DISActorMappings.Find(EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityID);
	if (associatedActor != nullptr && *associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
		UDISComponent* DISComponent = (*associatedActor)->FindComponentByClass<UDISComponent>();

		if (DISComponent != nullptr)
		{
			DISComponent->HandleEntityStateUpdatePDU(EntityStateUpdatePDUIn);
		}
	}
	else
	{
		//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
		if (EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityAppearance & (1 << 23))
		{
			UE_LOG(LogDISGameState, Log, TEXT("Received Entity State Update PDU with a Deactivated Entity Appearance for an entity that is not in the level. Ignoring the PDU. Entity ID: %s"), *EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityID.ToString());
			return;
		}

		//Form an Entity State from the Entity State Update PDU
		UGRILL_EntityStatePDU* entityState = NewObject<UGRILL_EntityStatePDU>();
		entityState->SetupFromEntityStateUpdatePDU(EntityStateUpdatePDUIn);

		SpawnNewEntityFromEntityState(entityState);
	}
}

void ADISGameState::HandleFirePDU(UGRILL_FirePDU* FirePDUIn)
{
	//Get associated OpenDISComponent and relay information
	UDISComponent* DISComponent = GetAssociatedDISComponent(FirePDUIn->FirePduStruct.MunitionEntityID);

	if (DISComponent != nullptr)
	{
		DISComponent->HandleFirePDU(FirePDUIn);
	}
}

void ADISGameState::HandleDetonationPDU(UGRILL_DetonationPDU* DetonationPDUIn)
{
	//Get associated OpenDISComponent and relay information
	UDISComponent* DISComponent = GetAssociatedDISComponent(DetonationPDUIn->DetonationPduStruct.MunitionEntityID);

	if (DISComponent != nullptr)
	{
		DISComponent->HandleDetonationPDU(DetonationPDUIn);
	}
}

void ADISGameState::HandleRemoveEntityPDU(UGRILL_RemoveEntityPDU* RemoveEntityPDUIn)
{
	//Verify that we are the appropriate sim to handle the RemoveEntityPDU
	if (RemoveEntityPDUIn->RemoveEntityPduStruct.ReceivingEntityID.Site == SiteID && RemoveEntityPDUIn->RemoveEntityPduStruct.ReceivingEntityID.Application == ApplicationID)
	{
		//Get associated OpenDISComponent and relay information
		UDISComponent* DISComponent = GetAssociatedDISComponent(RemoveEntityPDUIn->RemoveEntityPduStruct.ReceivingEntityID);

		if (DISComponent != nullptr)
		{
			DISComponent->HandleRemoveEntityPDU(RemoveEntityPDUIn);
		}
	}
}

void ADISGameState::SpawnNewEntityFromEntityState(UGRILL_EntityStatePDU* EntityStatePDUIn)
{
	//If an actor was not found -- check to see if there is an associated actor for the entity type
	TAssetSubclassOf<ADISEntity_Base>* associatedSoftClassReference = DISClassMappings.Find(EntityStatePDUIn->EntityStatePduStruct.EntityType);
	if (associatedSoftClassReference != nullptr)
	{
		//If so, spawn one and relay information to the associated component
		UClass* associatedClass = associatedSoftClassReference->LoadSynchronous();
		if (associatedClass != nullptr)
		{ 
			ADISEntity_Base* spawnedActor = GetWorld()->SpawnActor<ADISEntity_Base>(associatedClass);

			if (spawnedActor != nullptr)
			{
				spawnedActor->OnDestroyed.AddDynamic(this, &ADISGameState::HandleOnDISEntityDestroyed);

				//Get DIS Component of the newly spawned actor
				//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
				UDISComponent* DISComponent = spawnedActor->FindComponentByClass<UDISComponent>();
				//Add actor to the map
				AddDISEntityToMap(EntityStatePDUIn->EntityStatePduStruct.EntityID, spawnedActor);

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
		UE_LOG(LogDISGameState, Warning, TEXT("No mapping exists between an actor and the DIS enumeration of: %s"), *EntityStatePDUIn->EntityStatePduStruct.EntityType.ToString());
	}
}

UDISComponent* ADISGameState::GetAssociatedDISComponent(FEntityID EntityIDIn)
{
	UDISComponent* DISComponent = nullptr;

	//Find associated actor in the DISActorMappings map
	ADISEntity_Base** associatedActor = DISActorMappings.Find(EntityIDIn);
	if (associatedActor != nullptr && *associatedActor != nullptr)
	{
		//If an actor was found, relay information to the associated component
		//Leaving this way rather than getting component of ADISEntity_Base in case we go to a more agnostic approach for classes able to be used
		DISComponent = (*associatedActor)->FindComponentByClass<UDISComponent>();
	}

	return DISComponent;
}

void ADISGameState::AddDISEntityToMap(FEntityID EntityIDToAdd, ADISEntity_Base* EntityToAdd)
{
	DISActorMappings.Add(EntityIDToAdd, EntityToAdd);
}

bool ADISGameState::RemoveDISEntityFromMap(FEntityID EntityIDToRemove)
{
	int amountRemoved = DISActorMappings.Remove(EntityIDToRemove);

	return (amountRemoved > 0);
}