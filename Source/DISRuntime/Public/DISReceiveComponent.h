// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DISEnumsAndStructs.h"
#include "PDUMasterInclude.h"
#include "GeoReferencingSystem.h"
#include "DISReceiveComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDISReceiveComponent, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedEntityStatePDU, FEntityStatePDU, EntityStatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadReckoningUpdate, FEntityStatePDU, DeadReckonedEntityStatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedEntityStateUpdatePDU, FEntityStateUpdatePDU, EntityStateUpdatePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedDetonationPDU, FDetonationPDU, DetonationPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedFirePDU, FFirePDU, FirePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedRemoveEntityPDU, FRemoveEntityPDU, RemoveEntityPDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedStopFreezePDU, FStopFreezePDU, StopFreezePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceivedStartResumePDU, FStartResumePDU, StartResumePDU);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGroundClampingUpdate, TArray<FTransform>, ClampTransforms);

DECLARE_STATS_GROUP(TEXT("GRILLDIS_Game"), STATGROUP_DISComponent, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("DoDeadReckoning"), STAT_DoDeadReckoning, STATGROUP_DISComponent);
DECLARE_CYCLE_STAT(TEXT("GroundClamping"), STAT_GroundClamping, STATGROUP_DISComponent);

/**
 * The DISReceiveComponent handles basic receiving DIS functionality.
 * It should be attached to actors where receiving DIS is desired.
 * 
 * By default the DISReceiveComponent offers packet handling for the associated DIS entity, 
 * basic dead reckoning, basic ground clamping, and maintains a living list of appropriate
 * DIS variables (such as Entity ID).
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, DisplayName = "DIS Receive Component"))
class DISRUNTIME_API UDISReceiveComponent : public UActorComponent
{
	GENERATED_BODY()

	/**
	 * Clamps an entity to the ground. Verifies that the entity is of ground domain, is not a munition, and is owned by a different sim prior to clamping.
	 * Calls OnGroundClampingUpdate event when finished.
	 */
	virtual void GroundClamping_Implementation();

public:
	// Sets default values for this component's properties
	UDISReceiveComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;

	void HandleEntityStatePDU(FEntityStatePDU NewEntityStatePDU);
	void HandleEntityStateUpdatePDU(FEntityStateUpdatePDU NewEntityStateUpdatePDU);
	void HandleFirePDU(FFirePDU FirePDUIn);
	void HandleDetonationPDU(FDetonationPDU DetonationPDUIn);
	void HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn);
	void HandleStopFreezePDU(FStopFreezePDU StopFreezePDUIn);
	void HandleStartResumePDU(FStartResumePDU StartResumePDUIn);
	void DoDeadReckoning(float DeltaTime);

	/**
	 * Called after a dead reckoning update is performed by the component.
	 * Passes out an Entity State PDU with updated dead reckoning variables as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FDeadReckoningUpdate OnDeadReckoningUpdate;
	/**
	 * Called after an Entity State PDU is received by the component. The component updates associated variables prior to broadcasting this event.
	 * Passes the Entity State PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FReceivedEntityStatePDU OnReceivedEntityStatePDU;
	/**
	 * Called after an Entity State Update PDU is received by the component. The component updates associated variables prior to broadcasting this event.
	 * Passes the Entity State Update PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FReceivedEntityStateUpdatePDU OnReceivedEntityStateUpdatePDU;
	/**
	 * Called after a Detonation PDU is received by the component.
	 * Passes the Detonation PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FReceivedDetonationPDU OnReceivedDetonationPDU;
	/**
	 * Called after a Fire PDU is received by the component.
	 * Passes the Fire PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FReceivedFirePDU OnReceivedFirePDU;
	/**
	 * Called after a Remove Entity PDU is received by the component.
	 * Passes the Remove Entity PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FReceivedRemoveEntityPDU OnReceivedRemoveEntityPDU;
	/**
	 * Called after a Stop/Freeze PDU is received by the component.
	 * Passes the Stop/Freeze PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FReceivedStopFreezePDU OnReceivedStopFreezePDU;
	/**
	 * Called after a Start/Resume PDU is received by the component.
	 * Passes the Start/Resume PDU that was received as a parameter.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FReceivedStartResumePDU OnReceivedStartResumePDU;
	/**
	 * Called after Ground Clamping is performed by the component.
	 * Passes ground clamp transforms (if clamping multiple points) as a parameter.
	 * 
	 * NOTE: Gets called after receiving Entity State PDUs or finishing Dead Reckoning Updates if Ground Clamping is enabled on the DISComponent.
	 * Respective Entity State and Dead Reckoning events are called first. Implementing Ground Clamping location updates on top of these event may cause jitter in actor location.
	 */
	UPROPERTY(BlueprintAssignable, Category = "GRILL DIS|DIS Receive Component|Event")
		FGroundClampingUpdate OnGroundClampingUpdate;

	/**
	 * Clamps an entity to the ground. Should call OnGroundClampingUpdate event when finished.
	 */	
	UFUNCTION(BlueprintNativeEvent, Category = "GRILL DIS|DIS Receive Component")
		void GroundClamping();

	/**
	 * The most recent Entity State PDU that has been received.
	*/
	UPROPERTY(BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		FEntityStatePDU MostRecentEntityStatePDU;

	/**
	 * The most recent Dead Reckoned Entity State PDU that has been calculated.
	*/
	UPROPERTY(BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		FEntityStatePDU MostRecentDeadReckonedEntityStatePDU;

	/**
	 * The timestamp that the most recent Entity State PDU was received at by the DISComponent.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		FDateTime LatestEntityStatePDUTimestamp;
	/**
	 * Whether or not the associated entity was spawned by the network or not.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		bool SpawnedFromNetwork = false;
	/**
	 * The Entity Type of the associated entity. Specifies the kind of entity, the country of design, 
	 * the domain, the specific identification of the entity, and any extra information necessary for describing the entity.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		FEntityType EntityType;
	/**
	 * The Entity ID of the associated entity. Each Entity ID should be unique to an entity in the sim.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		FEntityID EntityID;
	/**
	 * The Force ID of the associated entity. Specifies the team or side the DIS entity is on.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		EForceID EntityForceID;
	/**
	 * The Entity Marking of the associated entity. Designates a friendly name for the DIS entity.
	 * Max of 11 characters should be used. If more than 11 are used, it will be truncated.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GRILL DIS|DIS Receive Component|DIS Info")
		FString EntityMarking;

	/**
	 * The time to live for the entity. Gets reset every time a new Entity State PDU is received by the sim.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings")
		float DISTimeoutSeconds = 30.0f;
	/**
	 * Determines what all DIS info should be culled. Allows for updates to happen less frequently for entities that aren't currently important.
	 * The distance that culling should happen is handled by the DISCullingDistance variable.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings")
		EDISCullingMode DISCullingMode = EDISCullingMode::None;
	/**
	 * The distance in Unreal units from the camera that culling should begin happening.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings")
		float DISCullingDistance = 0.0f;
	/**
	 * Whether or not dead reckoning should be performed for this entity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings")
		bool PerformDeadReckoning = true;
	/**
	 * Whether or not dead reckoning should be locally smoothed for this entity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings", meta = (EditCondition = "PerformDeadReckoning"))
		bool PerformDeadReckoningSmoothing = true;
	/**
	 * Number of seconds to smooth between dead reckoned information and packet information if dead reckoning is enabled
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings", meta = (EditCondition = "PerformDeadReckoning && PerformDeadReckoningSmoothing"))
		float DeadReckoningSmoothingPeriodSeconds = 0.5f;
	/**
	 * Determines how this entity should be ground clamped.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings")
		EGroundClampingMode PerformGroundClamping = EGroundClampingMode::GroundClampWithDISOptions;
	/**
	 * The collision channel to use for ground clamping.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Receive Component|DIS Settings")
		TEnumAsByte<ETraceTypeQuery> GoundClampingCollisionChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FVector EntityECEFLocationDifference;
	FRotator EntityRotationDifference;
	AGeoReferencingSystem* GeoReferencingSystem;

	FEntityStatePDU PreviousDeadReckonedPDU;
	float DeltaTimeSinceLastPDU = 0;
	int NumberEntityStatePDUsReceived = 0;

	void UpdateCommonEntityStateInfo(FEntityStatePDU NewEntityStatePDU);
	FEntityStatePDU SmoothDeadReckoning(FEntityStatePDU DeadReckonPDUToSmooth);
};