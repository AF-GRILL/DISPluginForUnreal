// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DISEnumsAndStructs.h"
#include "PDUMasterInclude.h"
#include "UDPSubsystem.h"
#include "Components/ActorComponent.h"
#include "DISSendComponent.generated.h"

//Forward declarations
class ADISGameManager;

DECLARE_LOG_CATEGORY_EXTERN(LogDISSendComponent, Log, All);

/**
 * The DISSendComponent handles basic sending DIS functionality.
 * It should be attached to actors where sending DIS is desired.
 *
 * By default the DISSendComponent offers automatic Entity State PDU sending
 * for the associated DIS Entity. Maintains a list of appropriate
 * DIS variables (such as Entity ID).
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, DisplayName = "DIS Send Component"))
class DISRUNTIME_API UDISSendComponent : public UActorComponent
{
	GENERATED_BODY()

		/**
		* Attempts to send an updated Entity State or Entity State Update PDU. Verifies if the entity is outside of the Dead Reckoning thresholds first.
		* Returns whether or not an updated PDU was sent.
		*/
		virtual bool SendEntityStatePDU_Implementation();

public:
	// Sets default values for this component's properties
	UDISSendComponent();

	/**
	* Updates the Capabilities that the entity has.
	* @param NewEntityCapabilities The new DIS capabilities that the entity has.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		void SetEntityCapabilities(int32 NewEntityCapabilities);

	/**
	* Updates the appearance of the entity.
	* @param NewEntityAppearance The new appearance that the entity has.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		void SetEntityAppearance(int32 NewEntityAppearance);

	/**
	* Updates the Dead Reckoning algorithm being used by the Send Component.
	* @param NewDeadReckoningAlgorithm The new dead reckoning algorithm to use.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		void SetDeadReckoningAlgorithm(int32 NewDeadReckoningAlgorithm);

	/**
	* Compares the current Dead Reckoning location/orientation to the actual location/orientation of the entity.
	* Returns whether or not the comparison is outside of the Dead Reckoning threshold.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		bool CheckDeadReckoningThreshold();

	/**
	* Forms an Entity State PDU based on current known information about the Entity.
	* Returns the Entity State PDU that was formed.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		FEntityStatePDU FormEntityStatePDU();

	/**
	* Checks whether or not the orientation threshold between the actual orientation and Dead Reckoning orientation has been exceeded. Utilizes quaternions to calculate it.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		bool CheckOrientationQuaternionThreshold();

	/**
	* Checks whether or not the orientation threshold between the actual orientation and Dead Reckoning orientation has been exceeded. Utilizes matrices to calculate it.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		bool CheckOrientationMatrixThreshold();

	/**
	* Calculates the linear velocity of the given locations using the given delta time. The FVector returned is in terms of ECEF.
	* @param UnrealLinearVelocity The linear velocity to be converted to ECEF coordinates
	* @param CurrentLocation The Unreal Engine location that the entity is at
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		FVector CalculateECEFLinearVelocity(FVector UnrealLinearVelocity, FVector CurrentLocation);

	/**
	* Attempts to send an updated Entity State or Entity State Update PDU. Should check Dead Reckoning threshold prior to sending.
	* Returns whether or not an update was sent.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "GRILL DIS|DIS Send Component")
		bool SendEntityStatePDU();

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
	 * The Entity Type of the associated entity. Specifies the kind of entity, the country of design,
	 * the domain, the specific identification of the entity, and any extra information necessary for describing the entity.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Info")
		FEntityType EntityType;
	/**
	 * The Entity ID of the associated entity. Each Entity ID should be unique to an entity in the sim.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Info")
		FEntityID EntityID;
	/**
	 * The Force ID of the associated entity. Specifies the team or side the DIS entity is on.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Info")
		EForceID EntityForceID;
	/**
	 * The Entity Marking of the associated entity. Designates a friendly name for the DIS entity.
	 * Max of 11 characters should be used. If more than 11 are used, it will be truncated.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Info")
		FString EntityMarking;

	/**
	 * The time to live for the entity. Gets reset every time a new Entity State PDU is received by the sim.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Settings")
		float DISHeartbeatSeconds = 5.0f;
	/**
	* The sending mode that this entity should use for Entity State PDUs.
	* Dictates if none are sent, Entity State PDUs are sent, or Entity State Update PDUs are sent.
	* Distribution of Entity State PDUs are handled internally by the DIS Component via the overridable "SendEntityStatePDU" function.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Settings")
		EEntityStateSendingMode EntityStatePDUSendingMode;

	/**
	* The dead reckoning algorithm to use. Specifies the dynamic changes to the entities appearance attributes.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0, ClampMin = 0))
		int32 EntityAppearance = 1;

	/**
	* The DIS Capabilities that the entity should have. Int representation of a collection of boolean fields which describe the capabilities of the entity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0, ClampMin = 0))
		int32 EntityCapabilities = 1;

	/**
	* The dead reckoning algorithm to use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 1, ClampMin = 1, UIMax = 9, ClampMax = 9))
		int32 DeadReckoningAlgorithm = 1;

	/**
	* The position threshold to use for dead reckoning. If the dead reckoning position deviates more than this value away from the actual position in any axis, a new Entity State PDU will be sent.
	* This value should be in meters.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0, ClampMin = 0))
		float DeadReckoningPositionThresholdMeters = 1;
	/**
	* The orientation threshold to use for dead reckoning. If the dead reckoning orientation deviates more than this value away from the actual orientation, a new Entity State PDU will be sent.
	* This value should be in degrees.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0, ClampMin = 0))
		float DeadReckoningOrientationThresholdDegrees = 3;

protected:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	* Takes in an Entity State PDU and emits it as an Entity State or Entity State Update PDU. Decides based on what EntityStatePDUSendingMode is set to.
	* @param pduToSend The Entity State PDU to emit.
	*/
	bool EmitAppropriatePDU(FEntityStatePDU pduToSend);

private:
	float DeltaTimeSinceLastPDU = 0;
	FEntityStatePDU PreviousEntityStatePDU;

	FVector PreviousUnrealLocation;
	FRotator PreviousUnrealRotation;
	FVector PreviousUnrealLinearVelocity;

	AGeoReferencingSystem* GeoReferencingSystem;
	ADISGameManager* DISGameManager;
	UUDPSubsystem* UDPSubsystem;
};
