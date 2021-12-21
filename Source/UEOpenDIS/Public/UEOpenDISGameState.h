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

	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
	void HandleEntityStatePDU(FEntityStatePDU EntityStatePDUIn);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
	void HandleFirePDU(FFirePDU FirePDUIn);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void HandleDetonationPDU(FDetonationPDU DetonationPDUIn);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn);

	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		void AddDISEntityToMap(FEntityID EntityIDToAdd, ADISEntity_Base* EntityToAdd);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS")
		bool RemoveDISEntityFromMap(FEntityID EntityIDToRemove);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void HandleOnDISEntityDestroyed(AActor* DestroyedActor);

	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityType, TAssetSubclassOf<ADISEntity_Base>> DISClassMappings;
	UPROPERTY(BlueprintReadOnly)
		TMap<FEntityID, ADISEntity_Base*> DISActorMappings;
	UPROPERTY(BlueprintReadOnly)
		int32 ExerciseID;
	UPROPERTY(BlueprintReadOnly)
		int32 SiteID;
	UPROPERTY(BlueprintReadOnly)
		int32 ApplicationID;

private:
	UOpenDISComponent* GetAssociatedOpenDISComponent(FEntityID EntityIDIn);
};