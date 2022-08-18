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
	PreviousUnrealLinearVelocity = (GetOwner()->GetActorLocation() - PreviousUnrealLocation) / (DeltaTime * 100);
	PreviousUnrealLocation = GetOwner()->GetActorLocation();
	PreviousUnrealRotation = GetOwner()->GetActorRotation();
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

	if (IsValid(UDPSubsystem) && EntityStatePDUSendingMode != EEntityStateSendingMode::None)
	{
		UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStatePDUToBytes(finalESPDU));
	}

	Super::EndPlay(EndPlayReason);
}

void UDISSendComponent::SetDeadReckoningAlgorithm(int32 NewDeadReckoningAlgorithm)
{
	//If the dead reckoning algorithm differs, send out a new ESPDU
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

	if (IsValid(DISGameManager))
	{
		newEntityStatePDU.ExerciseID = DISGameManager->ExerciseID;
	}
	else
	{
		UE_LOG(LogDISSendComponent, Warning, TEXT("Invalid DISGameManager. Please make sure one is in the world."));
	}

	if (IsValid(GeoReferencingSystem))
	{
		//Calculate the position of the entity in ECEF
		FVector ecefLocation;
		UDIS_BPFL::CalculateEcefXYZFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, ecefLocation);

		newEntityStatePDU.EcefLocation = FVector(ecefLocation.X, ecefLocation.Y, ecefLocation.Z);

		//Calculate the orientation of the entity in Psi, Theta, Phi
		FVector latLonHeightMeters;
		FHeadingPitchRoll headingPitchRollDegrees;
		FPsiThetaPhi psiThetaPhiRadians;
		UDIS_BPFL::CalculateLatLonHeightFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, latLonHeightMeters);
		UDIS_BPFL::GetHeadingPitchRollFromUnrealRotation(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation(), GeoReferencingSystem, headingPitchRollDegrees);
		UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonHeightMeters.X, latLonHeightMeters.Y, psiThetaPhiRadians);

		newEntityStatePDU.EntityOrientation = FRotator(psiThetaPhiRadians.Theta, psiThetaPhiRadians.Psi, psiThetaPhiRadians.Phi);
	}
	else
	{
		UE_LOG(LogDISSendComponent, Warning, TEXT("Invalid GeoReference. Please make sure one is in the world."));
	}

	newEntityStatePDU.DeadReckoningParameters.DeadReckoningAlgorithm = DeadReckoningAlgorithm;

	float deltaTime = GetWorld()->DeltaTimeSeconds;

	if (deltaTime > 0)
	{
		//Calculate the velocities and acceleration of the entity in m/s
		FVector curUnrealLinearVelocity = (GetOwner()->GetActorLocation() - PreviousUnrealLocation) / (deltaTime * 100);

		FRotator rotDiff = UDeadReckoning_BPFL::CalculateDirectionalRotationDifference(PreviousUnrealRotation, GetOwner()->GetActorRotation());

		FVector angularVelocity = FMath::DegreesToRadians(rotDiff.Euler()) / deltaTime;
		newEntityStatePDU.DeadReckoningParameters.EntityAngularVelocity = angularVelocity;

		FVector linearAcceleration = FVector::ZeroVector;
		//Apply the appropriate acceleration based on Dead Reckoning algorithm being used
		if (DeadReckoningAlgorithm < 6)
		{
			newEntityStatePDU.EntityLinearVelocity = CalculateECEFLinearVelocity(curUnrealLinearVelocity, GetOwner()->GetActorLocation());
			FVector prevECEFLinearVelocity = CalculateECEFLinearVelocity(PreviousUnrealLinearVelocity, GetOwner()->GetActorLocation());
			linearAcceleration = (newEntityStatePDU.EntityLinearVelocity - prevECEFLinearVelocity) / deltaTime;
		}
		else
		{
			//Convert linear velocity vectors to be in body space
			newEntityStatePDU.EntityLinearVelocity = UKismetMathLibrary::GreaterGreater_VectorRotator(curUnrealLinearVelocity, GetOwner()->GetActorRotation().GetInverse()) * FVector(1, 1, -1);
			FVector prevVelBodySpace = UKismetMathLibrary::GreaterGreater_VectorRotator(PreviousUnrealLinearVelocity, GetOwner()->GetActorRotation().GetInverse()) * FVector(1, 1, -1);
			linearAcceleration = (newEntityStatePDU.EntityLinearVelocity - prevVelBodySpace) / deltaTime;
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
		UDIS_BPFL::CalculateEcefXYZFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, ecefLocation);

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
	FQuat DR_OrientationQuaternion = UDeadReckoning_BPFL::MultiplyQuaternions(entityOrientationQuaternion, deadReckoningQuaternion);

	FQuat actualOrientationQuaternion;

	if (IsValid(GeoReferencingSystem))
	{
		//Calculate the orientation of the entity in Psi, Theta, Phi
		FVector latLonHeightMeters;
		FHeadingPitchRoll headingPitchRollDegrees;
		FPsiThetaPhi psiThetaPhiRadians;

		UDIS_BPFL::CalculateLatLonHeightFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, latLonHeightMeters);
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

		UDIS_BPFL::CalculateLatLonHeightFromUnrealLocation(GetOwner()->GetActorLocation(), GeoReferencingSystem, latLonHeightMeters);
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

FVector UDISSendComponent::CalculateECEFLinearVelocity(FVector UnrealLinearVelocity, FVector CurrentLocation)
{
	FVector linearVelocity = UnrealLinearVelocity;

	if (IsValid(GeoReferencingSystem))
	{
		FVector llh;
		FNorthEastDown nedVectors;
		UDIS_BPFL::CalculateLatLonHeightFromUnrealLocation(CurrentLocation, GeoReferencingSystem, llh);
		UDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(llh.X, llh.Y, nedVectors);

		//Convert the Unreal Engine linear velocity to be in terms of ECEF
		linearVelocity = nedVectors.NorthVector * -linearVelocity.Y + nedVectors.EastVector * linearVelocity.X - nedVectors.DownVector * linearVelocity.Z;
	}

	return linearVelocity;
}

bool UDISSendComponent::SendEntityStatePDU_Implementation()
{
	bool sentUpdate = false;

	//Verify a new Entity State or Entity State Update PDU should be sent
	if ((EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStatePDU || EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStateUpdatePDU) && (CheckDeadReckoningThreshold() || DeltaTimeSinceLastPDU > DISHeartbeatSeconds))
	{
		MostRecentEntityStatePDU = FormEntityStatePDU();
		MostRecentDeadReckonedEntityStatePDU = MostRecentEntityStatePDU;
		PreviousEntityStatePDU = MostRecentEntityStatePDU;

		//Send out the appropriate PDU
		if (IsValid(UDPSubsystem) && EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStatePDU)
		{
			UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStatePDUToBytes(MostRecentEntityStatePDU));
		}
		else if (IsValid(UDPSubsystem) && EntityStatePDUSendingMode == EEntityStateSendingMode::EntityStateUpdatePDU)
		{
			UDPSubsystem->EmitBytes(UPDUConversions_BPFL::ConvertEntityStateUpdatePDUToBytes(MostRecentEntityStatePDU.ToEntityStateUpdatePDU()));
		}

		sentUpdate = true;
		DeltaTimeSinceLastPDU = 0;
	}

	return sentUpdate;
}