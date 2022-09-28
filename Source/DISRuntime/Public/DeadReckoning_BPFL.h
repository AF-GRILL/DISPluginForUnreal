// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DISEnumsAndStructs.h"
#include "PDUMasterInclude.h"
#include "DIS_BPFL.h"
#include "glm/gtx/quaternion.hpp"
#include "DeadReckoning_BPFL.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDeadReckoning_BPFL, Log, All);

/**
 *
 */
UCLASS()
class DISRUNTIME_API UDeadReckoning_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Performs dead reckoning on the given Entity PDU.
	 * @param EntityPDUToDeadReckon The Entity State PDU to perform dead reckoning on.
	 * @param DeltaTime The time elapsed in seconds that dead reckoning should be calculated for.
	 * @param DeadReckonedEntityPDU The resulting dead reckoned EntityStatePDU that contains all of the dead reckoning information.
	*/
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Dead Reckoning")
		static bool DeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime, FEntityStatePDU& DeadReckonedEntityPDU);

	/**
	 * Forms the Other Parameters section utilized in Dead Reckoning Parameters.
	 * @param DeadReckoningAlgorithm The dead reckoning algorithm being used.
	 * @param EntityPsiThetaPhiRadians The Psi, Theta, Phi orientation of the entity.
	 * @param EntityECEFLocation The ECEF location of the entity.
	*/
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|Dead Reckoning")
		static TArray<uint8> FormOtherParameters(EDeadReckoningAlgorithm DeadReckoningAlgorithm, FRotator EntityPsiThetaPhiRadians, FVector EntityECEFLocation);

	/**
	 * Calculates and returns the dead reckoning matrix used for calculating entity rotation
	 * @param AngularVelocityVector The angular velocity vector in body coordinates
	 * @param DeltaTime The time increment for dead reckoning calculations
	 */
	static glm::dmat3 CreateDeadReckoningMatrix(glm::dvec3 AngularVelocityVector, double DeltaTime);

	/**
	 * Calculates and returns the dead reckoning quaternion used for calculating entity rotation
	 * @param AngularVelocityVector The angular velocity vector in body coordinates
	 * @param DeltaTime The time increment for dead reckoning calculations
	 */
	static FQuat CreateDeadReckoningQuaternion(glm::dvec3 AngularVelocityVector, double DeltaTime);

	/**
	 * Calculates a DIS entity's orientation matrix from the provided Euler angles
	 * @param PsiRadians The rotation about the Z axis in radians
	 * @param ThetaRadians The rotation about the Y axis in radians
	 * @param PhiRadians The rotation about the X axis in radians
	 */
	static glm::dmat3 GetEntityOrientationMatrix(double PsiRadians, double ThetaRadians, double PhiRadians);

	/**
	 * Calculates a DIS entity's orientation quaternion from the provided Euler angles
	 * @param PsiRadians The rotation about the Z axis in radians
	 * @param ThetaRadians The rotation about the Y axis in radians
	 * @param PhiRadians The rotation about the X axis in radians
	 */
	static FQuat GetEntityOrientationQuaternion(double PsiRadians, double ThetaRadians, double PhiRadians);

private:

	static const double MIN_ROTATION_RATE;

	/**
	 *Checks if the machine is in Little Endian.Returns true if Little Endian or false if Big Endian.
	*/
	static bool IsMachineLittleEndian();

	/**
	 * Calculates the Psi, Theta, Phi orientation for the entity from the given orientation quaternion. Utilized as a final step in converting quaternion OtherParameters to an orientation.
	 * @param AngularVelocityVector The angular velocity of the entity
	 * @param EntityRotationQuaternion The orientation quaternion to use to calculate the Psi, Theta, Phi orientation
	 * @param DeltaTime The time increment for dead reckoning calculation
	*/
	static FRotator CalculateDeadReckonedEulerAnglesFromQuaternion(glm::dvec3 AngularVelocityVector, FQuat EntityRotationQuaternion, float DeltaTime);

	/**
	 * Gets the local yaw, pitch, and roll from the other parameters structure. The yaw, pitch, and roll act on the entity's local North, East, Down vectors.
	 * @param OtherDeadReckoningParameters The 120 bits sent as part of the dead reckoning parameters marked as other parameters sent as an array of bytes
	 * @param LocalRotator The local yaw, pitch, and roll of the entity in radians. Yaw is the heading from true north, positive to the right. Pitch is the elevation angle above or below the local horizon, positive up. Roll is the bank angle from the local horizontal, positive tile to the right.
	 */
	static bool GetLocalEulerAngles(TArray<uint8> OtherDeadReckoningParameters, FRotator& LocalRotator);

	/**
	 * Gets the local entity orientation as a quaternion from the dead reckoning other parameters
	 * @param OtherDeadReckoningParameters The 120 bits sent as part of the dead reckoning parameters marked as other parameters sent as an array of bytes
	 * @param EntityOrientation The four-valued unit quaternion that represents the entity's orientation
	 */
	static bool GetLocalQuaternionAngles(TArray<uint8> OtherDeadReckoningParameters, FQuat& EntityOrientation);

	/**
	 * Calculates the new position vector using the given velocity, acceleration, and time increment
	 * @param PositionVector The initial position of the entity in world coordinates
	 * @param VelocityVector The initial velocity in world coordinates
	 * @param AccelerationVector The initial acceleration in world coordinates
	 * @param DeltaTime The time increment for dead reckoning calculation
	 */
	static glm::dvec3 CalculateDeadReckonedPosition(glm::dvec3 PositionVector, glm::dvec3 VelocityVector, glm::dvec3 AccelerationVector, double DeltaTime);

	/**
	 * Calculates the new position vector using the given velocity, acceleration, and time increment given in body coordinates
	 * @param InitialPositionVector The initial position of the entity in body coordinates
	 * @param BodyVelocityVector The initial velocity in body coordinates
	 * @param BodyLinearAccelerationVector The initial linear acceleration in body coordinates
	 * @param BodyAngularVelocityVector The initial angular velocity in body coordinates
	 * @param EntityOrientation The orientation of the entity (Psi[Yaw]/Theta[Pitch]/Phi[Roll]) in radians
	 * @param DeltaTime The time increment for dead reckoning calculation
	 */
	static glm::dvec3 GetEntityBodyDeadReckonedPosition(glm::dvec3 InitialPositionVector, glm::dvec3 BodyVelocityVector, glm::dvec3 BodyLinearAccelerationVector, glm::dvec3 BodyAngularVelocityVector, glm::dvec3 EntityOrientation, double DeltaTime);

	/**
	 * Calculates the new orientation vector using the given velocity, acceleration, and time increment
	 * @param PsiRadians The rotation about the Z axis in radians
	 * @param ThetaRadians The rotation about the Y axis in radians
	 * @param PhiRadians The rotation about the X axis in radians
	 * @param AngularVelocityVector The angular velocity vector in body coordinates
	 * @param DeltaTime The time increment for dead reckoning calculation
	 * @param OutPsiRadians The rotation about the Z axis in radians after time step DeltaTime
	 * @param OutThetaRadians The rotation about the Y axis in radians after time step DeltaTime
	 * @param OutPhiRadians The rotation about the X axis in radians after time step DeltaTime
	 */
	static void CalculateDeadReckonedOrientation(double PsiRadians, double ThetaRadians, double PhiRadians, glm::dvec3 AngularVelocityVector, float DeltaTime, double& OutPsiRadians, double& OutThetaRadians, double& OutPhiRadians);

	/**
	 * Converts the given local heading, pitch, roll rotator in radians into world space psi, theta, phi rotator in radians.
	 * @param ECEFLocation The ECEF location of the entity.
	 * @param LocalRotatorRadians The local heading, pitch, roll rotator in radians
	 * @param PsiThetaPhiRadians The world space psi, theta, phi rotator in radians
	*/
	static void ConvertLocalRotatorToPsiThetaPhiRadians(FVector ECEFLocation, FRotator LocalRotatorRadians, FPsiThetaPhi& PsiThetaPhiRadians);
};
