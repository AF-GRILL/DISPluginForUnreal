// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "DISEnumsAndStructs.h"
#include "PDUMasterInclude.h"
#include "GameFramework/GameStateBase.h"
#include "DISGameState.generated.h"

//Forward declarations
class UDISComponent;
class ADISEntity_Base;

DECLARE_LOG_CATEGORY_EXTERN(LogDISGameState, Log, All);

UCLASS(Blueprintable)
class DISRUNTIME_API ADISGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADISGameState();

	/**
	 * Delegates the given Entity State PDU to the appropriate DIS Entity actor.
	 * @param EntityStatePDUIn - The Entity State PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game State")
		void HandleEntityStatePDU(UGRILL_EntityStatePDU* EntityStatePDUIn);
	/**
	 * Delegates the given Entity State Update PDU to the appropriate DIS Entity actor.
	 * @param EntityStateUpdatePDUIn - The Entity State Update PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game State")
		void HandleEntityStateUpdatePDU(UGRILL_EntityStateUpdatePDU* EntityStateUpdatePDUIn);
	/**
	 * Delegates the given Fire PDU to the appropriate DIS Entity actor.
	 * @param FirePDUIn - The Fire PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game State")
		void HandleFirePDU(UGRILL_FirePDU* FirePDUIn);
	/**
	 * Delegates the given Detonation PDU to the appropriate DIS Entity actor.
	 * @param DetonationPDUIn - The Detonation PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game State")
		void HandleDetonationPDU(UGRILL_DetonationPDU* DetonationPDUIn);
	/**
	 * Delegates the given Remove Entity PDU to the appropriate DIS Entity actor.
	 * @param RemoveEntityPDUIn - The Remove Entity PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game State")
		void HandleRemoveEntityPDU(UGRILL_RemoveEntityPDU* RemoveEntityPDUIn);

	/**
	 * Adds a new entry to the DIS Entity map.
	 * @param EntityIDToAdd - The Entity ID to key the given entity under in the map.
	 * @param EntityToAdd - The entity to add to the entity map.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game State")
		void AddDISEntityToMap(FEntityID EntityIDToAdd, ADISEntity_Base* EntityToAdd);
	/**
	 * Removes the entry correlating to the given Entity ID from the DIS Entity map.
	 * Returns whether or not an entry was removed.
	 * @param EntityIDToRemove - The Entity ID of the DIS entity that needs removed from the entity map.
	 */
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|Game State")
		bool RemoveDISEntityFromMap(FEntityID EntityIDToRemove);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void HandleOnDISEntityDestroyed(AActor* DestroyedActor);
	
	/**
	 * The mapping between DIS Enumerations and corresponding entity classes.
	 */
	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityType, TAssetSubclassOf<ADISEntity_Base>> DISClassMappings;
	std::map<FEntityType, TAssetSubclassOf<ADISEntity_Base>> RawDISClassMappings;
	/**
	 * The mapping between DIS Entity IDs and corresponding entity actors.
	 */
	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityID, ADISEntity_Base*> DISActorMappings;
	std::map<FEntityID, ADISEntity_Base*> RawDISActorMappings;
	/**
	 * The exercise ID of the DIS sim.
	 */
	UPROPERTY(BlueprintReadWrite)
		int32 ExerciseID;
	/**
	 * The site ID of this application instance.
	 */
	UPROPERTY(BlueprintReadWrite)
		int32 SiteID;
	/**
	 * The application ID of this application instance.
	 */
	UPROPERTY(BlueprintReadWrite)
		int32 ApplicationID;

private:
	void SpawnNewEntityFromEntityState(UGRILL_EntityStatePDU* EntityStatePDUIn);
	UDISComponent* GetAssociatedDISComponent(FEntityID EntityIDIn);
};