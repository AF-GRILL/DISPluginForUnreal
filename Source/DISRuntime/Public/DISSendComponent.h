// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "EnumsAndStructs/DISEnumsAndStructs.h"
#include "PDUMasterInclude.h"
#include "UDPSubsystem.h"
#include "Components/ActorComponent.h"
#include "DISSendComponent.generated.h"

//Forward declarations
class ADISGameManager;
class AGeoReferencingSystem;

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
		void SetDeadReckoningAlgorithm(EDeadReckoningAlgorithm NewDeadReckoningAlgorithm);

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
	 * Attempts to send an updated Entity State or Entity State Update PDU. Checks Dead Reckoning threshold prior to sending.
	 * Returns whether or not an update was sent.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		bool SendEntityStatePDU();

	/**
	 * Calculates the Linear Velocity and Linear Acceleration of the entity that this component is attached to. Calculates them in terms of ECEF coordinates.
	 * @param DeltaTime The time that has passed since the last calculation.
	 * @param ECEFLinearVelocity The linear velocity of the entity in ECEF coordinates.
	 * @param ECEFLinearAcceleration The linear acceleration of the entity in ECEF coordinates
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		void CalculateECEFLinearVelocityAndAcceleration(FVector& ECEFLinearVelocity, FVector& ECEFLinearAcceleration);

	/**
	 * Calculates the Linear Velocity and Linear Acceleration of the entity that this component is attached to. Calculates them in terms of entity body coordinates.
	 * @param DeltaTime The time that has passed since the last calculation.
	 * @param AngularVelocity The current angular velocity of the entity.
	 * @param BodyLinearVelocity The linear velocity of the entity in Body coordinates.
	 * @param BodyLinearAcceleration The linear acceleration of the entity in Body coordinates
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		void CalculateBodyLinearVelocityAndAcceleration(FVector AngularVelocity, FVector& BodyLinearVelocity, FVector& BodyLinearAcceleration);

	/**
	 * Calculates the Angular Velocity of the entity that this component is attached to.
	 * Returns an FVector containing the angular velocity in radians per second.
	*/
	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|DIS Send Component")
		FVector CalculateAngularVelocity();

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
	 * How often an Entity State PDU should be sent out for this entity.
	 * Timer gets reset if an Entity State PDU gets sent out by this component by another means (ex: threshold clipping).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0, ClampMin = 0))
		float DISHeartbeatSeconds = 5.0f;
	/**
	 * The sending mode that this entity should use for Entity State PDUs.
	 * Dictates if none are sent, Entity State PDUs are sent, or Entity State Update PDUs are sent.
	 * Distribution of Entity State PDUs are handled internally by the DIS Component via the overridable "SendEntityStatePDU" function.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|DIS Send Component|DIS Settings")
		EEntityStateSendingMode EntityStatePDUSendingMode;

	/**
	 * The rate at which the timer that calculates the current linear velocity, linear acceleration, and angular acceleration of the entity gets executed.
	 * The values calculated by this timer get utilized when forming an Entity State PDU.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0.01, ClampMin = 0.01))
		float EntityStateCalculationRate = 0.1f;

	/**
	 * The dead reckoning algorithm to use. Specifies the dynamic changes to the entities appearance attributes.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0, ClampMin = 0))
		int32 EntityAppearance = 0;

	/**
	 * The DIS Capabilities that the entity should have. Int representation of a collection of boolean fields which describe the capabilities of the entity.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GRILL DIS|DIS Send Component|DIS Settings", Meta = (UIMin = 0, ClampMin = 0))
		int32 EntityCapabilities = 0;

	/**
	 * The dead reckoning algorithm to use.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GRILL DIS|DIS Send Component|DIS Settings")
		EDeadReckoningAlgorithm DeadReckoningAlgorithm = EDeadReckoningAlgorithm::Static;

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
	 * Updates last calculated linear velocity, linear acceleration, and angular velocity.
	*/
	void UpdateEntityStateCalculations();

	/**
	 * Takes in an Entity State PDU and emits it as an Entity State or Entity State Update PDU. Decides based on what EntityStatePDUSendingMode is set to.
	 * @param pduToSend The Entity State PDU to emit.
	*/
	bool EmitAppropriatePDU(FEntityStatePDU pduToSend);

private:
	float DeltaTimeSinceLastPDU = 0;

	FTimerHandle UpdateEntityStateCalculationsHandle;
	float TimeOfLastParametersCalculation;

	FVector LastCalculatedUnrealLocation;
	FRotator LastCalculatedUnrealRotation;
	FVector LastCalculatedUnrealLinearVelocity;
	FVector LastCalculatedECEFLinearVelocity;
	FVector LastCalculatedECEFLinearAcceleration;
	FVector LastCalculatedBodyLinearVelocity;
	FVector LastCalculatedBodyLinearAcceleration;
	FVector LastCalculatedAngularVelocity;

	AGeoReferencingSystem* GeoReferencingSystem;
	ADISGameManager* DISGameManager;
	UUDPSubsystem* UDPSubsystem;
};
