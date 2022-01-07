// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DISEnumsAndStructs.h"
#include "OpenDISComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOpenDISComponent, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedEntityStatePDU, FEntityStatePDU, EntityStatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedDetonationPDU, FDetonationPDU, DetonationPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedFirePDU, FFirePDU, FirePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedRemoveEntityPDU, FRemoveEntityPDU, RemoveEntityPDU);

/**
 * The OpenDISComponent handles basic DIS functionality.
 * It should be attached to actors where DIS interoperability is desired.
 * 
 * By default the OpenDISComponent offers packet handling for the associated DIS entity, 
 * basic dead reckoning, basic ground clamping, and maintains a living list of appropriate
 * DIS variables (such as Entity ID).
 * 
 * Currently should be used in conjunction with the DISEntity_Base actor to get full functionality.
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEOPENDIS_API UOpenDISComponent : public UActorComponent
{
	GENERATED_BODY()

	bool DeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime, FEntityStatePDU& DeadReckonedEntityPDU);

	FRotator GetRotationForDeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime);

public:
	// Sets default values for this component's properties
	UOpenDISComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HandleEntityStatePDU(FEntityStatePDU NewEntityStatePDU);
	void HandleFirePDU(FFirePDU FirePDUIn);
	void HandleDetonationPDU(FDetonationPDU DetonationPDUIn);
	void HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn);

	/**
	 * Called after a dead reckoning update is performed by the component.
	 * Passes out an Entity State PDU with updated dead reckoning variables as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedEntityStatePDU OnDeadReckoningUpdate;
	/**
	 * Called after an Entity State PDU is received by the component. The component updates associated variables prior to broadcasting this event.
	 * Passes the Entity State PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedEntityStatePDU OnReceivedEntityStatePDU;
	/**
	 * Called after a Detonation PDU is received by the component.
	 * Passes the Detonation PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedDetonationPDU OnReceivedDetonationPDU;
	/**
	 * Called after a Fire PDU is received by the component.
	 * Passes the Fire PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedFirePDU OnReceivedFirePDU;
	/**
	 * Called after a Remove Entity PDU is received by the component.
	 * Passes the Remove Entity PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Event")
		FReceivedRemoveEntityPDU OnReceivedRemoveEntityPDU;

	/**
	 * Clamps an entity to the ground. Verifies that the entity is of ground domain and is not a munition prior to clamping.
	 * Returns whether or not ground clamping was successful.
	 * @param EntityClampDirection - The direction vector to ground clamp the entity towards.
	 * @param ClampLocation - The location to ground clamp to.
	 * @param ClampRotation - The rotation to ground clamp to.
	 */
	UFUNCTION(BlueprintCallable)
		bool SimpleGroundClamping(FVector EntityClampDirection, FVector& ClampLocation, FRotator& ClampRotation);
	
	/**
	 * The most recent Entity State PDU that has been received by the OpenDISComponent.
	 */
	UPROPERTY(BlueprintReadWrite)
		FEntityStatePDU mostRecentEntityStatePDU;
	/**
	 * The timestamp that the most recent Entity State PDU was received at by the OpenDISComponent.
	 */
	UPROPERTY(BlueprintReadWrite)
		FDateTime latestPDUTimestamp;
	/**
	 * Whether or not the associated entity was spawned by the network or not.
	 */
	UPROPERTY(BlueprintReadWrite)
		bool SpawnedFromNetwork = false;

	/**
	 * The time to live for the entity. Gets reset every time a new Entity State PDU is received by the sim.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Info")
		float DISHeartbeat = 30.0f;
	/**
	 * The Entity Type of the associated entity. Specifies the kind of entity, the country of design, 
	 * the domain, the specific identification of the entity, and any extra information necessary for describing the entity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Info")
		FEntityType EntityType;
	/**
	 * The Entity ID of the associated entity. Each Entity ID should be unique to an entity in the sim.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Info")
		FEntityID EntityID;
	/**
	 * Whether or not dead reckoning should be performed for this entity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Settings")
		bool PerformDeadReckoning = true;
	/**
	 * The collision channel to use for ground clamping.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DIS Settings")
		TEnumAsByte<ETraceTypeQuery> GoundClampingCollisionChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FEntityStatePDU deadReckonedPDU;

};