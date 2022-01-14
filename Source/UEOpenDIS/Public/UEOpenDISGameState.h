// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DISEnumsAndStructs.h"
#include "GameFramework/GameStateBase.h"
#include "UEOpenDISGameState.generated.h"

//Forward declarations
class UOpenDISComponent;
class ADISEntity_Base;

DECLARE_LOG_CATEGORY_EXTERN(LogOpenDIS, Log, All);

UCLASS(Blueprintable)
class UEOPENDIS_API AUEOpenDISGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AUEOpenDISGameState();

	/**
	 * Delegates the given Entity State PDU to the appropriate DIS Entity actor.
	 * @param EntityStatePDUIn - The Entity State PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void HandleEntityStatePDU(FEntityStatePDU EntityStatePDUIn);
	/**
	 * Delegates the given Entity State Update PDU to the appropriate DIS Entity actor.
	 * @param EntityStateUpdatePDUIn - The Entity State Update PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void HandleEntityStateUpdatePDU(FEntityStateUpdatePDU EntityStateUpdatePDUIn);
	/**
	 * Delegates the given Fire PDU to the appropriate DIS Entity actor.
	 * @param FirePDUIn - The Fire PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void HandleFirePDU(FFirePDU FirePDUIn);
	/**
	 * Delegates the given Detonation PDU to the appropriate DIS Entity actor.
	 * @param DetonationPDUIn - The Detonation PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void HandleDetonationPDU(FDetonationPDU DetonationPDUIn);
	/**
	 * Delegates the given Remove Entity PDU to the appropriate DIS Entity actor.
	 * @param RemoveEntityPDUIn - The Remove Entity PDU to pass to the appropriate entity.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn);

	/**
	 * Adds a new entry to the DIS Entity map.
	 * @param EntityIDToAdd - The Entity ID to key the given entity under in the map.
	 * @param EntityToAdd - The entity to add to the entity map.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void AddDISEntityToMap(FEntityID EntityIDToAdd, ADISEntity_Base* EntityToAdd);
	/**
	 * Removes the entry correlating to the given Entity ID from the DIS Entity map.
	 * Returns whether or not an entry was removed.
	 * @param EntityIDToRemove - The Entity ID of the DIS entity that needs removed from the entity map.
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
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
	/**
	 * The mapping between DIS Entity IDs and corresponding entity actors.
	 */
	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityID, ADISEntity_Base*> DISActorMappings;
	/**
	 * The exercise ID of the DIS sim.
	 */
	UPROPERTY(BlueprintReadOnly)
		int32 ExerciseID;
	/**
	 * The site ID of this application instance.
	 */
	UPROPERTY(BlueprintReadOnly)
		int32 SiteID;
	/**
	 * The application ID of this application instance.
	 */
	UPROPERTY(BlueprintReadOnly)
		int32 ApplicationID;

private:
	void SpawnNewEntityFromEntityState(FEntityStatePDU EntityStatePDUIn);
	UOpenDISComponent* GetAssociatedOpenDISComponent(FEntityID EntityIDIn);
};