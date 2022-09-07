// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.


#include "DISSendComponent.h"

#include "DISGameManager.h"
#include "DeadReckoning_BPFL.h"
#include "PDUConversions_BPFL.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogDISSendComponent);

// Sets default values for this component's properties
UDISSendComponent::UDISSendComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UDISSendComponent::BeginPlay()
{
	Super::BeginPlay();

	GeoReferencingSystem = AGeoReferencingSystem::GetGeoReferencingSystem(Cast<UObject>(GetWorld()));
	DISGameManager = ADISGameManager::GetDISGameManager(Cast<UObject>(GetWorld()));

	//Get the UDP Subsystem and bind to receiving UDP Bytes
	UDPSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUDPSubsystem>();

	//Setup Previous/Current Location and Rotation variables.
	PreviousUnrealLocation = GetOwner()->GetActorLocation();
	PreviousUnrealRotation = GetOwner()->GetActorRotation();

	//Form Entity State PDU packets
	MostRecentEntityStatePDU = FormEntityStatePDU();
	MostRecentDeadReckonedEntityStatePDU = MostRecentEntityStatePDU;
	PreviousEntityStatePDU = MostRecentEntityStatePDU;

	//Begin play with Entity State PDU
	if (IsValid(UDPSubsystem) && EntityStatePDUSendingMode != EEntityStateSendingMode::None)
	{
		UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStatePDUToBytes(MostRecentEntityStatePDU));
	}
}

// Called every frame
void UDISSendComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DeltaTimeSinceLastPDU += DeltaTime;

	if (EntityStatePDUSendingMode != EEntityStateSendingMode::None)
	{
		SendEntityStatePDU();
	}

	//Update previous velocity, rotation, and location regardless of if an Entity State PDU was sent out.
	//Multiply by 100 to convert velocity to m/s
	PreviousUnrealLinearVelocity = (GetOwner()->GetActorLocation() - PreviousUnrealLocation) / (DeltaTime * 100);
	PreviousUnrealLocation = GetOwner()->GetActorLocation();
	PreviousUnrealRotation = GetOwner()->GetActorRotation();
	TimeOfLastVelocityCalculation = GetOwner()->GetGameTimeSinceCreation();
}

void UDISSendComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Emit a final EntityStatePDU detailing that the entity has been deactivated
	FEntityStatePDU finalESPDU = FEntityStatePDU();

	finalESPDU.EntityID = EntityID;
	finalESPDU.EntityType = EntityType;
	finalESPDU.ForceID = EntityForceID;
	finalESPDU.Marking = EntityMarking;
	finalESPDU.EntityAppearance |= 1 << 23;

	if (IsValid(DISGameManager))
	{
		finalESPDU.ExerciseID = DISGameManager->ExerciseID;
	}
	else
	{
		UE_LOG(LogDISSendComponent, Warning, TEXT("Invalid DISGameManager. Please make sure one is in the world."));
	}

	EmitAppropriatePDU(finalESPDU);

	Super::EndPlay(EndPlayReason);
}

void UDISSendComponent::SetEntityCapabilities(int32 NewEntityCapabilities)
{
	//If the new entity capabilities differ, send out a new ESPDU
	if (EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStatePDU && NewEntityCapabilities != EntityCapabilities && NewEntityCapabilities >= 0)
	{
		EntityCapabilities = NewEntityCapabilities;

		MostRecentEntityStatePDU = FormEntityStatePDU();
		MostRecentDeadReckonedEntityStatePDU = MostRecentEntityStatePDU;
		PreviousEntityStatePDU = MostRecentEntityStatePDU;

		if (IsValid(UDPSubsystem))
		{
			UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStatePDUToBytes(MostRecentEntityStatePDU));
		}
	}
}

void UDISSendComponent::SetEntityAppearance(int32 NewEntityAppearance)
{
	//If the new appearance differs, send out a new ESPDU
	if (NewEntityAppearance != EntityAppearance && NewEntityAppearance >= 0)
	{
		EntityAppearance = NewEntityAppearance;

		MostRecentEntityStatePDU = FormEntityStatePDU();
		MostRecentDeadReckonedEntityStatePDU = MostRecentEntityStatePDU;
		PreviousEntityStatePDU = MostRecentEntityStatePDU;

		EmitAppropriatePDU(MostRecentEntityStatePDU);
	}
}

void UDISSendComponent::SetDeadReckoningAlgorithm(EDeadReckoningAlgorithm NewDeadReckoningAlgorithm)
{
	//If the dead reckoning algorithm differs and is in the appropriate range, send out a new ESPDU
	if (EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStatePDU && NewDeadReckoningAlgorithm != DeadReckoningAlgorithm)
	{
		DeadReckoningAlgorithm = NewDeadReckoningAlgorithm;

		MostRecentEntityStatePDU = FormEntityStatePDU();
		MostRecentDeadReckonedEntityStatePDU = MostRecentEntityStatePDU;
		PreviousEntityStatePDU = MostRecentEntityStatePDU;

		if (IsValid(UDPSubsystem))
		{
			UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStatePDUToBytes(MostRecentEntityStatePDU));
		}
	}
}

FEntityStatePDU UDISSendComponent::FormEntityStatePDU()
{
	FEntityStatePDU newEntityStatePDU;

	newEntityStatePDU.EntityID = EntityID;
	newEntityStatePDU.EntityType = EntityType;
	newEntityStatePDU.ForceID = EntityForceID;
	newEntityStatePDU.Marking = EntityMarking;
	newEntityStatePDU.Capabilities = EntityCapabilities;
	newEntityStatePDU.EntityAppearance = EntityAppearance;

	newEntityStatePDU.DeadReckoningParameters.DeadReckoningAlgorithm = DeadReckoningAlgorithm;
	float timeSinceLastCalc = GetOwner()->GetGameTimeSinceCreation() - TimeOfLastVelocityCalculation;

	if (IsValid(DISGameManager))
	{
		newEntityStatePDU.ExerciseID = DISGameManager->ExerciseID;
	}
	else
	{
		UE_LOG(LogDISSendComponent, Warning, TEXT("Invalid DISGameManager. Please make sure one is in the world."));
	}

	//Set all geospatial values
	if (IsValid(GeoReferencingSystem))
	{
		//Calculate the position of the entity in ECEF
		FVector ecefLocation;
		UDIS_BPFL::GetEcefXYZFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, ecefLocation);

		newEntityStatePDU.EcefLocation = FVector(ecefLocation.X, ecefLocation.Y, ecefLocation.Z);

		//Calculate the orientation of the entity in Psi, Theta, Phi
		FVector latLonHeightMeters;
		FHeadingPitchRoll headingPitchRollDegrees;
		FPsiThetaPhi psiThetaPhiRadians;
		UDIS_BPFL::GetLatLonHeightFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, latLonHeightMeters);
		UDIS_BPFL::GetHeadingPitchRollFromUnrealRotation(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation(), GeoReferencingSystem, headingPitchRollDegrees);
		UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonHeightMeters.X, latLonHeightMeters.Y, psiThetaPhiRadians);

		newEntityStatePDU.EntityOrientation = FRotator(psiThetaPhiRadians.Theta, psiThetaPhiRadians.Psi, psiThetaPhiRadians.Phi);
	}
	else
	{
		UE_LOG(LogDISSendComponent, Warning, TEXT("Invalid GeoReference. Please make sure one is in the world."));
	}

	//Set all Dead Reckoning Parameters
	if (timeSinceLastCalc > 0)
	{
		//Calculate the velocities and acceleration of the entity in m/s
		FVector curLoc = GetOwner()->GetActorLocation();
		FVector curUnrealLinearVelocity = (curLoc - PreviousUnrealLocation) / (timeSinceLastCalc * 100);

		FRotator curRot = GetOwner()->GetActorRotation();
		FRotator rotDiff = UDeadReckoning_BPFL::CalculateDirectionalRotationDifference(PreviousUnrealRotation, curRot);

		FVector angularVelocity = FMath::DegreesToRadians(rotDiff.Euler()) / timeSinceLastCalc;
		newEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity = angularVelocity;

		FVector linearAcceleration = FVector::ZeroVector;
		//Apply the appropriate acceleration based on Dead Reckoning algorithm being used
		if ((uint8)DeadReckoningAlgorithm < 6)
		{
			//Convert linear velocity vectors to be in ECEF coordinates --- UE origin may not be Earth center and may lie rotated on Earth
			newEntityStatePDU.EntityLinearVelocity = UDIS_BPFL::ConvertUnrealVectorToECEFVector(curUnrealLinearVelocity, curLoc, GeoReferencingSystem);
			FVector prevECEFLinearVelocity = UDIS_BPFL::ConvertUnrealVectorToECEFVector(PreviousUnrealLinearVelocity, PreviousUnrealLocation, GeoReferencingSystem);
			linearAcceleration = (newEntityStatePDU.EntityLinearVelocity - prevECEFLinearVelocity) / timeSinceLastCalc;
		}
		else
		{
			//Convert linear velocity vectors to be in body space --- Use inverse UE rotations to convert vectors into appropriate DIS body space
			newEntityStatePDU.EntityLinearVelocity = UKismetMathLibrary::GreaterGreater_VectorRotator(curUnrealLinearVelocity, curRot.GetInverse()) * FVector(1, 1, -1);
			FVector prevVelBodySpace = UKismetMathLibrary::GreaterGreater_VectorRotator(PreviousUnrealLinearVelocity, PreviousUnrealRotation.GetInverse()) * FVector(1, 1, -1);
			linearAcceleration = (newEntityStatePDU.EntityLinearVelocity - prevVelBodySpace) / timeSinceLastCalc;

			//Calculate the centripetal acceleration in body space
			glm::dvec3 dvecAngularVelocity = glm::dvec3(angularVelocity.X, angularVelocity.Y, angularVelocity.Z);
			glm::dmat3x3 SkewMatrix = UDIS_BPFL::CreateNCrossXMatrix(dvecAngularVelocity);
			glm::dvec3 dvecBodyVelocityVector = glm::dvec3(newEntityStatePDU.EntityLinearVelocity.X, newEntityStatePDU.EntityLinearVelocity.Y, newEntityStatePDU.EntityLinearVelocity.Z);
			glm::dvec3 dvecCentripetalAcceleration = (SkewMatrix * dvecBodyVelocityVector);

			//Add in body centripetal acceleration. Will get removed by the body dead reckoning algorithm
			linearAcceleration += FVector(dvecCentripetalAcceleration.x, dvecCentripetalAcceleration.y, dvecCentripetalAcceleration.z);
		}

		newEntityStatePDU.DeadReckoningParameters.EntityLinearAcceleration = linearAcceleration;
	}

	newEntityStatePDU.DeadReckoningParameters.OtherParameters = UDeadReckoning_BPFL::FormOtherParameters(DeadReckoningAlgorithm, newEntityStatePDU.EntityOrientation, newEntityStatePDU.EcefLocation);

	return newEntityStatePDU;
}

bool UDISSendComponent::CheckDeadReckoningThreshold()
{
	bool outsideThreshold = false;

	if (UDeadReckoning_BPFL::DeadReckoning(MostRecentEntityStatePDU, DeltaTimeSinceLastPDU, MostRecentDeadReckonedEntityStatePDU))
	{
		//Get the actual position of the entity
		FVector ecefLocation;
		UDIS_BPFL::GetEcefXYZFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, ecefLocation);

		//Get the position difference along each axis. Values should be in ECEF.
		bool xPosOutsideThreshold = abs(ecefLocation.X - MostRecentDeadReckonedEntityStatePDU.EcefLocation.X) > DeadReckoningPositionThresholdMeters;
		bool yPosOutsideThreshold = abs(ecefLocation.Y - MostRecentDeadReckonedEntityStatePDU.EcefLocation.Y) > DeadReckoningPositionThresholdMeters;
		bool zPosOutsideThreshold = abs(ecefLocation.Z - MostRecentDeadReckonedEntityStatePDU.EcefLocation.Z) > DeadReckoningPositionThresholdMeters;

		//Check if the position difference is beyond the position threshold in any axis
		if (xPosOutsideThreshold || yPosOutsideThreshold || zPosOutsideThreshold || CheckOrientationQuaternionThreshold())
		{
			outsideThreshold = true;
		}
	}

	return outsideThreshold;
}

bool UDISSendComponent::CheckOrientationQuaternionThreshold()
{
	bool outsideThreshold = false;

	glm::dvec3 AngularVelocityVector = glm::dvec3(PreviousEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity.X,
		PreviousEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity.Y, PreviousEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity.Z);

	//Get the entity orientation quaternion
	FQuat entityOrientationQuaternion = UDeadReckoning_BPFL::GetEntityOrientationQuaternion(PreviousEntityStatePDU.EntityOrientation.Yaw, PreviousEntityStatePDU.EntityOrientation.Pitch, PreviousEntityStatePDU.EntityOrientation.Roll);
	//Get the entity dead reckoning quaternion
	FQuat deadReckoningQuaternion = UDeadReckoning_BPFL::CreateDeadReckoningQuaternion(AngularVelocityVector, DeltaTimeSinceLastPDU);
	//Calculate the new orientation quaternion
	FQuat DR_OrientationQuaternion = entityOrientationQuaternion * deadReckoningQuaternion;

	FQuat actualOrientationQuaternion;

	if (IsValid(GeoReferencingSystem))
	{
		//Calculate the orientation of the entity in Psi, Theta, Phi
		FVector latLonHeightMeters;
		FHeadingPitchRoll headingPitchRollDegrees;
		FPsiThetaPhi psiThetaPhiRadians;

		UDIS_BPFL::GetLatLonHeightFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, latLonHeightMeters);
		UDIS_BPFL::GetHeadingPitchRollFromUnrealRotation(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation(), GeoReferencingSystem, headingPitchRollDegrees);
		UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonHeightMeters.X, latLonHeightMeters.Y, psiThetaPhiRadians);
		// Get the entity's current orientation quaternion
		actualOrientationQuaternion = UDeadReckoning_BPFL::GetEntityOrientationQuaternion(psiThetaPhiRadians.Psi, psiThetaPhiRadians.Theta, psiThetaPhiRadians.Phi);
	}
	else
	{
		UE_LOG(LogDISSendComponent, Warning, TEXT("Invalid GeoReference. Please make sure one is in the world."));
		return false;
	}

	float quaternionDotProduct = actualOrientationQuaternion.operator|(DR_OrientationQuaternion);

	double OrientationQuaternionThresholdEpsilon = 1 - FMath::Cos(FMath::DegreesToRadians(DeadReckoningOrientationThresholdDegrees / 2));

	//Check if outside of threshold -- 1 is chosen so that left hand side will be 0 if rotations do not differ
	outsideThreshold = (1 - quaternionDotProduct) > OrientationQuaternionThresholdEpsilon;

	return outsideThreshold;
}

bool UDISSendComponent::CheckOrientationMatrixThreshold()
{
	bool outsideThreshold = false;

	glm::dvec3 AngularVelocityVector = glm::dvec3(PreviousEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity.X,
		PreviousEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity.Y, PreviousEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity.Z);

	// Get the entity's current orientation matrix
	auto OrientationMatrix = UDeadReckoning_BPFL::GetEntityOrientationMatrix(PreviousEntityStatePDU.EntityOrientation.Yaw, PreviousEntityStatePDU.EntityOrientation.Pitch, PreviousEntityStatePDU.EntityOrientation.Roll);
	// Get the change in rotation for this time step
	const auto DeadReckoningMatrix = UDeadReckoning_BPFL::CreateDeadReckoningMatrix(AngularVelocityVector, DeltaTimeSinceLastPDU);
	// Calculate the new orientation matrix
	auto DR_OrientationMatrix = DeadReckoningMatrix * OrientationMatrix;

	glm::dmat3 ActualOrientationMatrix;

	if (IsValid(GeoReferencingSystem))
	{
		//Calculate the orientation of the entity in Psi, Theta, Phi
		FVector latLonHeightMeters;
		FHeadingPitchRoll headingPitchRollDegrees;
		FPsiThetaPhi psiThetaPhiRadians;

		UDIS_BPFL::GetLatLonHeightFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, latLonHeightMeters);
		UDIS_BPFL::GetHeadingPitchRollFromUnrealRotation(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation(), GeoReferencingSystem, headingPitchRollDegrees);
		UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonHeightMeters.X, latLonHeightMeters.Y, psiThetaPhiRadians);
		// Get the entity's current orientation matrix
		ActualOrientationMatrix = UDeadReckoning_BPFL::GetEntityOrientationMatrix(psiThetaPhiRadians.Psi, psiThetaPhiRadians.Theta, psiThetaPhiRadians.Phi);
	}
	else
	{
		UE_LOG(LogDISSendComponent, Warning, TEXT("Invalid GeoReference. Please make sure one is in the world."));
		return false;
	}

	//Calculate the rotational difference matrix and its trace
	auto rotDiffMatrix = glm::transpose(DR_OrientationMatrix) * ActualOrientationMatrix;
	float rotDiffTrace = rotDiffMatrix[0][0] + rotDiffMatrix[1][1] + rotDiffMatrix[2][2];

	double OrientationMatrixThresholdDelta = 2 - 2 * FMath::Cos(FMath::DegreesToRadians(DeadReckoningOrientationThresholdDegrees));

	//Check if outside of threshold -- 3 is chosen so that left hand side will be 0 if rotations do not differ (that is if the rotDiffMatrix was an identity matrix)
	outsideThreshold = (3 - rotDiffTrace) > OrientationMatrixThresholdDelta;

	return outsideThreshold;
}

bool UDISSendComponent::SendEntityStatePDU_Implementation()
{
	bool sentUpdate = false;

	//Verify a new Entity State or Entity State Update PDU should be sent
	if ((EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStatePDU || EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStateUpdatePDU) && (DeltaTimeSinceLastPDU > DISHeartbeatSeconds || CheckDeadReckoningThreshold()))
	{
		MostRecentEntityStatePDU = FormEntityStatePDU();
		MostRecentDeadReckonedEntityStatePDU = MostRecentEntityStatePDU;
		PreviousEntityStatePDU = MostRecentEntityStatePDU;

		EmitAppropriatePDU(MostRecentEntityStatePDU);

		sentUpdate = true;
		DeltaTimeSinceLastPDU = 0;
	}

	return sentUpdate;
}

bool UDISSendComponent::EmitAppropriatePDU(FEntityStatePDU pduToSend)
{
	bool successful = false;
	//Send out the appropriate PDU
	if (IsValid(UDPSubsystem) && EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStatePDU)
	{
		successful = UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStatePDUToBytes(pduToSend));
	}
	else if (IsValid(UDPSubsystem) && EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStateUpdatePDU)
	{
		successful = UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStateUpdatePDUToBytes(pduToSend.ToEntityStateUpdatePDU()));
	}

	return successful;
}