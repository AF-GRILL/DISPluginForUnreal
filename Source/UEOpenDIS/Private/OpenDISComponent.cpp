// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDISComponent.h"

#include "UEOpenDIS_BPFL.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogOpenDISComponent);

// Sets default values for this component's properties
UOpenDISComponent::UOpenDISComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDISComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UOpenDISComponent::GetLocalEulerAngles(TArray<uint8> OtherDeadReckoningParameters, FRotator& LocalRotator)
{
	// Ensure the DR Parameter type is set to 1
	if (OtherDeadReckoningParameters[0] != 1) return;

	// Ensure the Array is at least 15 bytes long
	if (OtherDeadReckoningParameters.Num() < 15) return;

	// The next 2 bytes are padding and not necessary so skip indices 1 and 2
	// Concatenate the next three groups of four bytes
	float LocalYaw = (OtherDeadReckoningParameters[3] << 24) + (OtherDeadReckoningParameters[4] << 16) + (OtherDeadReckoningParameters[5] << 8) + (OtherDeadReckoningParameters[6]);
	float LocalPitch = (OtherDeadReckoningParameters[7] << 24) + (OtherDeadReckoningParameters[8] << 16) + (OtherDeadReckoningParameters[9] << 8) + (OtherDeadReckoningParameters[10]);
	float LocalRoll = (OtherDeadReckoningParameters[11] << 24) + (OtherDeadReckoningParameters[12] << 16) + (OtherDeadReckoningParameters[13] << 8) + (OtherDeadReckoningParameters[14]);

	// Convert each angle from radians to degrees for FRotator
	LocalYaw = FMath::RadiansToDegrees(LocalYaw);
	LocalPitch = FMath::RadiansToDegrees(LocalPitch);
	LocalRoll = FMath::RadiansToDegrees(LocalRoll);

	// Set the values and return
	LocalRotator = FRotator(LocalPitch, LocalYaw, LocalRoll);
}

void UOpenDISComponent::GetLocalQuaternionAngles(TArray<uint8> OtherDeadReckoningParameters, FQuat& EntityOrientation)
{
	// Ensure the DR Parameter type is set to 2
	if (OtherDeadReckoningParameters[0] != 2) return;

	// Ensure the array is at least 15 bytes long
	if (OtherDeadReckoningParameters.Num() < 15) return;

	//The next two bytes represent the 16 bit unsigned int approximation of q_0 (q_w in UE4 terminology)
	uint16 Qu0 = (OtherDeadReckoningParameters[1] << 8) + OtherDeadReckoningParameters[2];

	// The x, y, and z components of the quaternion are the next three groups of four bytes
	float QuX = (OtherDeadReckoningParameters[3] << 24) + (OtherDeadReckoningParameters[4] << 16) + (OtherDeadReckoningParameters[5] << 8) + (OtherDeadReckoningParameters[6]);
	float QuY = (OtherDeadReckoningParameters[7] << 24) + (OtherDeadReckoningParameters[8] << 16) + (OtherDeadReckoningParameters[9] << 8) + (OtherDeadReckoningParameters[10]);
	float QuZ = (OtherDeadReckoningParameters[11] << 24) + (OtherDeadReckoningParameters[12] << 16) + (OtherDeadReckoningParameters[13] << 8) + (OtherDeadReckoningParameters[14]);

	// Calculate the appropriate Qu0
	Qu0 = FMath::Sqrt(1 - (FMath::Square(QuX) + FMath::Square(QuY) + FMath::Square(QuZ)));

	// Set the values and return
	EntityOrientation = FQuat(QuX, QuY, QuZ, Qu0);
}

glm::dvec3 UOpenDISComponent::CalculateDeadReckonedPosition(const glm::dvec3 PositionVector, const glm::dvec3 VelocityVector,
	const glm::dvec3 AccelerationVector, const double DeltaTime)
{
	return PositionVector + (VelocityVector * DeltaTime) + ((1. / 2.) * AccelerationVector * FMath::Square(DeltaTime));
}

glm::dmat3 UOpenDISComponent::CreateDeadReckoningMatrix(glm::dvec3 AngularVelocityVector, double DeltaTime)
{
	const double AngularVelocityMagnitude = glm::length(AngularVelocityVector);

	const auto AngularVelocityMatrix = glm::dmat3(AngularVelocityVector, glm::dvec3(0), glm::dvec3(0));
	const auto AngularVelocity = AngularVelocityMatrix * glm::transpose(AngularVelocityMatrix);

	const auto CosOmega = glm::cos(AngularVelocityMagnitude * DeltaTime);
	const auto SinOmega = glm::sin(AngularVelocityMagnitude * DeltaTime);

	glm::dmat3 DeadReckoningMatrix = (((1 - CosOmega) / glm::pow(AngularVelocityMagnitude, 2)) * AngularVelocity) +
									(CosOmega * glm::dmat3()) +
									(SinOmega / AngularVelocityMagnitude * UUEOpenDIS_BPFL::CreateNCrossXMatrix(AngularVelocityVector));

	return DeadReckoningMatrix;
}

glm::dmat3 UOpenDISComponent::GetEntityOrientationMatrix(const double PsiRadians, const double ThetaRadians, const double PhiRadians)
{
	auto OrientationMatrix = glm::dmat3();
	OrientationMatrix[0][0] = glm::cos(ThetaRadians) * glm::cos(PsiRadians);
	OrientationMatrix[0][1] = glm::cos(ThetaRadians) * glm::sin(PsiRadians);
	OrientationMatrix[0][2] = -glm::sin(ThetaRadians);
	OrientationMatrix[1][0] = glm::sin(PhiRadians) * glm::sin(ThetaRadians) * glm::cos(PsiRadians) - glm::cos(PhiRadians) * glm::sin(PsiRadians);
	OrientationMatrix[1][1] = glm::sin(PhiRadians) * glm::sin(ThetaRadians) * glm::sin(PsiRadians) + glm::cos(PhiRadians) * glm::cos(PsiRadians);
	OrientationMatrix[1][2] = glm::sin(PhiRadians) * glm::cos(ThetaRadians);
	OrientationMatrix[2][0] = glm::cos(PhiRadians) * glm::sin(ThetaRadians) * glm::cos(PsiRadians) + glm::sin(PhiRadians) * glm::sin(PsiRadians);
	OrientationMatrix[2][1] = glm::cos(PhiRadians) * glm::sin(ThetaRadians) * glm::sin(PsiRadians) - glm::sin(PhiRadians) * glm::cos(PsiRadians);
	OrientationMatrix[2][2] = glm::cos(PhiRadians) * glm::cos(ThetaRadians);

	return OrientationMatrix;

}

void UOpenDISComponent::CalculateDeadReckonedOrientation(const double PsiRadians, const double ThetaRadians, const double PhiRadians,
	const glm::dvec3 AngularVelocityVector, const float DeltaTime, double &OutPsiRadians, double &OutThetaRadians, double &OutPhiRadians)
{
	// Get the entity's current orientation matrix
	auto OrientationMatrix = GetEntityOrientationMatrix(PsiRadians, ThetaRadians, PhiRadians);

	// Get the change in rotation for this time step
	const auto DeadReckoningMatrix = CreateDeadReckoningMatrix(AngularVelocityVector, DeltaTime);

	// Calculate the new orientation matrix
	OrientationMatrix = DeadReckoningMatrix * OrientationMatrix;

	// Extract Euler angles from orientation matrix
	OutThetaRadians = glm::asin(-OrientationMatrix[0][2]);

	// Special case for |Theta| = pi/2
	double CosTheta = 1e-5;
	if (abs(OutThetaRadians) != glm::pi<double>()/2)
	{
		CosTheta = glm::cos(OutThetaRadians);
	}

	OutPsiRadians = glm::acos(OrientationMatrix[0][0] / CosTheta) * (abs(OrientationMatrix[0][1]) / OrientationMatrix[0][1]);
	OutPhiRadians = glm::acos(OrientationMatrix[2][2] / CosTheta) * (abs(OrientationMatrix[1][2]) / OrientationMatrix[1][2]);
}

// Called every frame
void UOpenDISComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check if dead reckoning is supported/enabled. Broadcast dead reckoning update if it is
	if (DeadReckoning(DeadReckoningEntityStatePDU, DeltaTime, deadReckonedPDU))
	{
		DeadReckoningEntityStatePDU = deadReckonedPDU;
		OnDeadReckoningUpdate.Broadcast(deadReckonedPDU);
	}
}

void UOpenDISComponent::HandleEntityStatePDU(FEntityStatePDU NewEntityStatePDU)
{
	//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
	if (NewEntityStatePDU.EntityAppearance & (1 << 23))
	{
		UE_LOG(LogOpenDISComponent, Log, TEXT("%s Entity Appearance is set to deactivated, deleting entity..."), *NewEntityStatePDU.Marking);
		GetOwner()->Destroy();
	}

	latestPDUTimestamp = FDateTime::Now();
	mostRecentEntityStatePDU = NewEntityStatePDU;
	DeadReckoningEntityStatePDU = mostRecentEntityStatePDU;

	EntityType = NewEntityStatePDU.EntityType;
	EntityID = NewEntityStatePDU.EntityID;

	GetOwner()->SetLifeSpan(DISHeartbeat);

	OnReceivedEntityStatePDU.Broadcast(NewEntityStatePDU);
}

void UOpenDISComponent::HandleEntityStateUpdatePDU(FEntityStateUpdatePDU NewEntityStateUpdatePDU)
{
	//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
	if (NewEntityStateUpdatePDU.EntityAppearance & (1 << 23))
	{
		UE_LOG(LogOpenDISComponent, Log, TEXT("%s Entity Appearance is set to deactivated, deleting entity..."), *NewEntityStateUpdatePDU.EntityID.ToString());
		GetOwner()->Destroy();
	}

	//Only modify the fields that are shared between the Entity State PDU and Entity State Update PDU. This will cover if the entity has received a full-up Entity State PDU already.
	mostRecentEntityStatePDU.EntityID = NewEntityStateUpdatePDU.EntityID;
	mostRecentEntityStatePDU.EntityLocationDouble = NewEntityStateUpdatePDU.EntityLocationDouble;
	mostRecentEntityStatePDU.EntityLocation = NewEntityStateUpdatePDU.EntityLocation;
	mostRecentEntityStatePDU.EntityOrientation = NewEntityStateUpdatePDU.EntityOrientation;
	mostRecentEntityStatePDU.EntityLinearVelocity = NewEntityStateUpdatePDU.EntityLinearVelocity;
	mostRecentEntityStatePDU.NumberOfArticulationParameters = NewEntityStateUpdatePDU.NumberOfArticulationParameters;
	mostRecentEntityStatePDU.EntityAppearance = NewEntityStateUpdatePDU.EntityAppearance;
	mostRecentEntityStatePDU.ArticulationParameters = NewEntityStateUpdatePDU.ArticulationParameters;

	latestPDUTimestamp = FDateTime::Now();
	mostRecentEntityStatePDU = NewEntityStateUpdatePDU;
	DeadReckoningEntityStatePDU = mostRecentEntityStatePDU;

	EntityID = NewEntityStateUpdatePDU.EntityID;

	GetOwner()->SetLifeSpan(DISHeartbeat);

	OnReceivedEntityStateUpdatePDU.Broadcast(NewEntityStateUpdatePDU);
}

void UOpenDISComponent::HandleFirePDU(FFirePDU FirePDUIn)
{
	OnReceivedFirePDU.Broadcast(FirePDUIn);
}

void UOpenDISComponent::HandleDetonationPDU(FDetonationPDU DetonationPDUIn)
{
	OnReceivedDetonationPDU.Broadcast(DetonationPDUIn);
}

void UOpenDISComponent::HandleRemoveEntityPDU(FRemoveEntityPDU RemoveEntityPDUIn)
{
	OnReceivedRemoveEntityPDU.Broadcast(RemoveEntityPDUIn);
}

bool UOpenDISComponent::DeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime, FEntityStatePDU& DeadReckonedEntityPDU)
{
	//Check if dead reckoning should be performed and if the entity is owned by another sim on the network
	//If not, then don't do dead reckoning
	if (!(PerformDeadReckoning && SpawnedFromNetwork))
	{
		PerformDeadReckoning = false;
		return false;
	}

	DeadReckonedEntityPDU = EntityPDUToDeadReckon;
	bool bSupported = true;

	switch (EntityPDUToDeadReckon.DeadReckoningParameters.DeadReckoningAlgorithm) {
	case 1: // Static
		{
			FRotator LocalRotator;
			GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

			bSupported = false;
			break;
		}

	case 2: // Fixed Position World (FPW)
		{
			// Set entity orientation
			FRotator LocalRotator;
			GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, glm::dvec3(0), DeltaTime);			

			DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];			

			DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

			break;
		}
	
	case 3: // Rotation Position World (RPW)
		{
			FQuat EntityRotation;
			GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotation);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, glm::dvec3(0), DeltaTime);

			DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

			glm::dvec3 AngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.X, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Z);
			//NOTE: Roll=Psi, Pitch=Theta, Yaw=Phi
			double PsiRadians, ThetaRadians, PhiRadians;
			CalculateDeadReckonedOrientation(EntityPDUToDeadReckon.EntityOrientation.Roll, EntityPDUToDeadReckon.EntityOrientation.Pitch, EntityPDUToDeadReckon.EntityOrientation.Yaw, AngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

			DeadReckonedEntityPDU.EntityOrientation.Roll = glm::degrees(PsiRadians);
			DeadReckonedEntityPDU.EntityOrientation.Pitch = glm::degrees(ThetaRadians);
			DeadReckonedEntityPDU.EntityOrientation.Yaw = glm::degrees(PhiRadians);

			break;
		}
	
	case 4: // Rotation Velocity World (RVW)
		{
			FQuat EntityRotation;
			GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotation);

			DeadReckonedEntityPDU.EntityOrientation = GetRotationForDeadReckoning(EntityPDUToDeadReckon, DeltaTime);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
			glm::dvec3 AccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, AccelerationVector, DeltaTime);

			DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

			glm::dvec3 AngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.X, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Z);
			//NOTE: Roll=Psi, Pitch=Theta, Yaw=Phi
			double PsiRadians, ThetaRadians, PhiRadians;
			CalculateDeadReckonedOrientation(EntityPDUToDeadReckon.EntityOrientation.Roll, EntityPDUToDeadReckon.EntityOrientation.Pitch, EntityPDUToDeadReckon.EntityOrientation.Yaw, AngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

			double Latitude, Longitude, Height;
			UUEOpenDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(DeadReckonedEntityPDU.EntityLocationDouble[0], DeadReckonedEntityPDU.EntityLocationDouble[1], DeadReckonedEntityPDU.EntityLocationDouble[2], Latitude, Longitude, Height);

			float Heading, Pitch, Roll;
			UUEOpenDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(PsiRadians, ThetaRadians, PhiRadians, Latitude, Longitude, Heading, Pitch, Roll);
			FVector NorthVector, EastVector, DownVector, TargetXVector, TargetYVector, TargetZVector;
			UUEOpenDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(Latitude, Longitude, NorthVector, EastVector, DownVector);
			UUEOpenDIS_BPFL::ApplyHeadingPitchRollToNorthEastDownVector(Heading, Pitch, Roll, NorthVector, EastVector, DownVector, TargetXVector, TargetYVector, TargetZVector);
			auto TargetRotation = FTransform(TargetXVector, TargetYVector, -TargetZVector, FVector(0));
			
			DeadReckonedEntityPDU.EntityOrientation = TargetRotation.Rotator();

			break;
		}
	
	case 5: // Fixed Velocity World (FVW)
		{
			FRotator LocalRotator;
			GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
			glm::dvec3 AccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, AccelerationVector, DeltaTime);

			DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

			break;
		}

	case 6: // Fixed Position Body (FPB)
		{
			FRotator LocalRotator;
			GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);
			bSupported = false;
			break;
		}

	case 7: // Rotation Position Body (RPB)
		{
			FQuat EntityRotation;
			GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotation);
			bSupported = false;
			break;
		}

	case 8: // Rotation Velocity Body (RVB)
		{
			FQuat EntityRotation;
			GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotation);
			bSupported = false;
			break;
		}

	case 9: // Fixed Velocity Body (FVB)
		{
			FRotator LocalRotator;
			GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);
			bSupported = false;
			break;
		}

	default: // Unknown
		{
			bSupported = false;
			break;
		}
	}

	return bSupported;
}

FRotator UOpenDISComponent::GetRotationForDeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime)
{
	// TODO: Implement DeadReckoning for the rotation change of the actor.
	// This should probably stay in Euler and leave conversions from euler to UE4 rotation to the end user of the plugin.

	return EntityPDUToDeadReckon.EntityOrientation;
}

bool UOpenDISComponent::SimpleGroundClamping(FVector EntityClampDirection, FVector& ClampLocation, FRotator& ClampRotation)
{
	bool groundClampSuccessful = false;

	//Verify that the entity is of the ground domain, and that it is not a munition
	if (EntityType.Domain == 1 && EntityType.EntityKind != 2)
	{
		EntityClampDirection.Normalize();

		FHitResult lineTraceHitResult;
		FVector actorLocation = GetOwner()->GetActorLocation();
		FVector endLocation = (EntityClampDirection * 100000) + actorLocation;
		FVector aboveActorStartLocation = (EntityClampDirection * -100000) + actorLocation;

		FCollisionQueryParams queryParams = FCollisionQueryParams(FName("Ground Clamping"), false, GetOwner());
		//Find colliding point above/below the actor
		if (GetWorld()->LineTraceSingleByChannel(lineTraceHitResult, aboveActorStartLocation, endLocation, UEngineTypes::ConvertToCollisionChannel(GoundClampingCollisionChannel), queryParams))
		{
			ClampLocation = lineTraceHitResult.Location;
			//Calculate what the new forward and right vectors should be based on the impact normal
			FVector newForward = FVector::CrossProduct(GetOwner()->GetActorRightVector(), lineTraceHitResult.ImpactNormal);
			FVector newRight = FVector::CrossProduct(lineTraceHitResult.ImpactNormal, newForward);

			ClampRotation = UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, lineTraceHitResult.ImpactNormal);

			groundClampSuccessful = true;
		}
	}

	return groundClampSuccessful;
}