// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UEOpenDISGameState.h"
#include "OpenDISComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedEntityStatePDU, FEntityStatePDU, EntityStatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedDetonationPDU, FDetonationPDU, DetonationPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedFirePDU, FFirePDU, FirePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedRemoveEntityPDU, FRemoveEntityPDU, RemoveEntityPDU);

UCLASS( Blueprintable, ClassGroup = (Custom), meta=(BlueprintSpawnableComponent) )
class UEOPENDIS_API UOpenDISComponent : public UActorComponent
{
	GENERATED_BODY()

	bool DeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime, FEntityStatePDU& DeadReckonedEntityPDU);

	FRotator GetRotationForDeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime);	

	virtual bool ApplyGroundClamping_Implementation(FVector& ClampLocation, FRotator& ClampRotation);
	

public:	
	// Sets default values for this component's properties
	UOpenDISComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleEntityStatePDU(FEntityStatePDU NewEntityStatePDU);
	void HandleFirePDU(FFirePDU FirePDUIn);
	void HandleDetonationPDU(FDetonationPDU DetonationPDUIn);
	void HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn);

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedEntityStatePDU OnDeadReckoningUpdate;
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedEntityStatePDU OnReceivedEntityStatePDU;	
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedDetonationPDU OnReceivedDetonationPDU;
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedFirePDU OnReceivedFirePDU;
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedRemoveEntityPDU OnReceivedRemoveEntityPDU;
	
	UFUNCTION(BlueprintNativeEvent)
		bool ApplyGroundClamping(FVector& ClampLocation, FRotator& ClampRotation);

	UPROPERTY(BlueprintReadWrite)
		FEntityStatePDU mostRecentEntityStatePDU;
	UPROPERTY(BlueprintReadWrite)
		FDateTime latestPDUTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Info")
		float DISHeartbeat = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Info")
		FEntityType EntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Info")
		FEntityID EntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Settings")
		bool PerformDeadReckoning = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Settings")
		bool PerformGroundClamping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Settings")
		TEnumAsByte<ECollisionChannel> GoundClampingCollisionChannel = ECollisionChannel::ECC_Visibility;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FEntityStatePDU deadReckonedPDU;	
	
};