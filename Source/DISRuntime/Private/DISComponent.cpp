// Fill out your copyright notice in the Description page of Project Settings.

#include "DISComponent.h"

#include "DIS_BPFL.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogDISComponent);

// Sets default values for this component's properties
UDISComponent::UDISComponent()
{
	MostRecentEntityStatePDU = NewObject<UGRILL_EntityStatePDU>();
	DeadReckoningEntityStatePDU = NewObject<UGRILL_EntityStatePDU>();
	TempDeadReckonedPDU = NewObject<UGRILL_EntityStatePDU>();

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called when the game starts
void UDISComponent::BeginPlay()
{
	Super::BeginPlay();
}

UGRILL_EntityStatePDU* UDISComponent::GetMostRecentEntityStatePDU()
{
	if (!IsValid(MostRecentEntityStatePDU))
	{
		MostRecentEntityStatePDU = NewObject<UGRILL_EntityStatePDU>();
	}

	return MostRecentEntityStatePDU;
}

UGRILL_EntityStatePDU* UDISComponent::GetMostRecentDeadReckoningPDU()
{
	if (!IsValid(DeadReckoningEntityStatePDU))
	{
		DeadReckoningEntityStatePDU = NewObject<UGRILL_EntityStatePDU>();
	}

	return DeadReckoningEntityStatePDU;
}

void UDISComponent::SetMostRecentEntityStatePDU(UPARAM(ref)UGRILL_EntityStatePDU*& EntityStatePDUIn)
{
	MostRecentEntityStatePDU = EntityStatePDUIn;
}

void UDISComponent::SetMostRecentDeadReckoningPDU(UPARAM(ref)UGRILL_EntityStatePDU*& DeadReckonedPDUIn)
{
	MostRecentEntityStatePDU = DeadReckonedPDUIn;
}


bool UDISComponent::GetLocalEulerAngles(TArray<uint8> OtherDeadReckoningParameters, FRotator& LocalRotator)
{
	// Ensure the Array is at least 15 bytes long
	if (OtherDeadReckoningParameters.Num() < 15) return false;

	// Ensure the DR Parameter type is set to 1
	if (OtherDeadReckoningParameters[0] != 1) return false;


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

	return true;
}

bool UDISComponent::GetLocalQuaternionAngles(TArray<uint8> OtherDeadReckoningParameters, FQuat& EntityOrientation)
{
	// Ensure the array is at least 15 bytes long
	if (OtherDeadReckoningParameters.Num() < 15) return false;

	// Ensure the DR Parameter type is set to 2
	if (OtherDeadReckoningParameters[0] != 2) return false;

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

	return true;
}

glm::dvec3 UDISComponent::CalculateDeadReckonedPosition(const glm::dvec3 PositionVector, const glm::dvec3 VelocityVector,
	const glm::dvec3 AccelerationVector, const double DeltaTime)
{
	return PositionVector + (VelocityVector * DeltaTime) + ((1. / 2.) * AccelerationVector * FMath::Square(DeltaTime));
}

glm::dmat3 UDISComponent::CreateDeadReckoningMatrix(glm::dvec3 AngularVelocityVector, double DeltaTime)
{
	const double AngularVelocityMagnitude = glm::length(AngularVelocityVector);

	const auto AngularVelocityMatrix = glm::dmat3(AngularVelocityVector, glm::dvec3(0), glm::dvec3(0));
	const auto AngularVelocity = AngularVelocityMatrix * glm::transpose(AngularVelocityMatrix);

	const auto CosOmega = glm::cos(AngularVelocityMagnitude * DeltaTime);
	const auto SinOmega = glm::sin(AngularVelocityMagnitude * DeltaTime);

	glm::dmat3 DeadReckoningMatrix = (((1 - CosOmega) / glm::pow(AngularVelocityMagnitude, 2)) * AngularVelocity) +
									(CosOmega * glm::dmat3(1)) -
									(SinOmega / AngularVelocityMagnitude * UDIS_BPFL::CreateNCrossXMatrix(AngularVelocityVector));

	return DeadReckoningMatrix;
}

glm::dmat3 UDISComponent::GetEntityOrientationMatrix(const double PsiRadians, const double ThetaRadians, const double PhiRadians)
{
	//Trig orientations
	const auto CosPsi = glm::cos(PsiRadians);
	const auto SinPsi = glm::sin(PsiRadians);
	const auto CosTheta = glm::cos(ThetaRadians);
	const auto SinTheta = glm::sin(ThetaRadians);
	const auto CosPhi = glm::cos(PhiRadians);
	const auto SinPhi = glm::sin(PhiRadians);

	const auto HeadingRotationMatrix = glm::dmat3(CosPsi, -SinPsi, 0, SinPsi, CosPsi, 0, 0, 0, 1);
	const auto PitchRotationMatrix = glm::dmat3(CosTheta, 0, SinTheta, 0, 1, 0, -SinTheta, 0, CosTheta);
	const auto RollRotationMatrix = glm::dmat3(1, 0, 0, 0, CosPhi, -SinPhi, 0, SinPhi, CosPhi);

	return RollRotationMatrix * PitchRotationMatrix * HeadingRotationMatrix;

}

void UDISComponent::CalculateDeadReckonedOrientation(const double PsiRadians, const double ThetaRadians, const double PhiRadians,
	const glm::dvec3 AngularVelocityVector, const float DeltaTime, double &OutPsiRadians, double &OutThetaRadians, double &OutPhiRadians)
{
	// Get the entity's current orientation matrix
	auto OrientationMatrix = GetEntityOrientationMatrix(PsiRadians, ThetaRadians, PhiRadians);

	// Get the change in rotation for this time step
	const auto DeadReckoningMatrix = CreateDeadReckoningMatrix(AngularVelocityVector, DeltaTime);

	// Calculate the new orientation matrix
	OrientationMatrix = DeadReckoningMatrix * OrientationMatrix;

	// Extract Euler angles from orientation matrix
	OutThetaRadians = glm::asin(-OrientationMatrix[2][0]);

	// Special case for |Theta| = pi/2
	double CosTheta = 1e-5;
	if (abs(OutThetaRadians) != glm::pi<double>()/2)
	{
		CosTheta = glm::cos(OutThetaRadians);
	}

	OutPsiRadians = glm::acos(OrientationMatrix[0][0] / CosTheta) * (abs(OrientationMatrix[1][0]) / OrientationMatrix[1][0]);
	OutPhiRadians = glm::acos(OrientationMatrix[2][2] / CosTheta) * (abs(OrientationMatrix[2][1]) / OrientationMatrix[2][1]);
}

glm::dvec3 UDISComponent::GetEntityBodyDeadReckonedPosition(const glm::dvec3 InitialPositionVector, const glm::dvec3 BodyVelocityVector, const glm::dvec3 BodyLinearAccelerationVector, const glm::dvec3 BodyAngularAccelerationVector, const glm::dvec3 EntityOrientation, const double DeltaTime)
{
	const auto SkewMatrix = UDIS_BPFL::CreateNCrossXMatrix(BodyAngularAccelerationVector);
	const auto BodyAccelerationVector = BodyLinearAccelerationVector - (SkewMatrix * BodyVelocityVector);

	// Get the entity's current orientation matrix
	const auto OrientationMatrix = GetEntityOrientationMatrix(EntityOrientation.x, EntityOrientation.y, EntityOrientation.z);
	const auto InverseInitialOrientationMatrix = inverse(OrientationMatrix);

	// Calculate R1
	const auto OmegaMatrix = glm::dmat3x3(BodyAngularAccelerationVector, glm::dvec3(0), glm::dvec3(0)) * glm::transpose(glm::dmat3x3(BodyAngularAccelerationVector, glm::dvec3(0), glm::dvec3(0)));
	constexpr auto AccelerationMagnitude = BodyAngularAccelerationVector.length();
	const auto R1 = (((AccelerationMagnitude * DeltaTime - glm::sin(AccelerationMagnitude * DeltaTime)) / glm::pow(AccelerationMagnitude, 3)) * OmegaMatrix) +
		(static_cast<double>(glm::sin(AccelerationMagnitude * DeltaTime) / AccelerationMagnitude) * glm::dmat3(1)) +
		(((1 - glm::cos(AccelerationMagnitude * DeltaTime)) / glm::pow(AccelerationMagnitude, 2)) * SkewMatrix);

	const auto R2 = ((((0.5 * glm::pow(AccelerationMagnitude, 2) * glm::pow(DeltaTime, 2)) - (glm::cos(AccelerationMagnitude * DeltaTime)) - (AccelerationMagnitude * DeltaTime * glm::sin(AccelerationMagnitude * DeltaTime)) + (1)) / glm::pow(AccelerationMagnitude, 4)) * SkewMatrix * glm::transpose(SkewMatrix)) +
		(static_cast<double>(((glm::cos(AccelerationMagnitude * DeltaTime)) + (AccelerationMagnitude * DeltaTime * glm::sin(AccelerationMagnitude * DeltaTime)) - (1)) / (glm::pow(AccelerationMagnitude, 2))) * glm::dmat3(1)) +
		((((glm::sin(AccelerationMagnitude * DeltaTime)) - (AccelerationMagnitude * DeltaTime * glm::cos(AccelerationMagnitude * DeltaTime))) / (glm::pow(AccelerationMagnitude, 3))) * SkewMatrix);

	return InitialPositionVector + (InverseInitialOrientationMatrix * ((R1 * BodyVelocityVector) + (R2 * BodyAccelerationVector)));
}

// Called every frame
void UDISComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check if dead reckoning is supported/enabled. Broadcast dead reckoning update if it is
	if (DeadReckoning(DeadReckoningEntityStatePDU, DeltaTime, TempDeadReckonedPDU))
	{
		DeadReckoningEntityStatePDU = TempDeadReckonedPDU;
		OnDeadReckoningUpdate.Broadcast(DeadReckoningEntityStatePDU);

		GroundClamping_Implementation();
	}
}

void UDISComponent::HandleEntityStatePDU(UGRILL_EntityStatePDU* NewEntityStatePDU)
{
	//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
	if (NewEntityStatePDU->EntityStatePDUStruct.EntityAppearance & (1 << 23))
	{
		UE_LOG(LogDISComponent, Log, TEXT("%s Entity Appearance is set to deactivated, deleting entity..."), *NewEntityStatePDU->EntityStatePDUStruct.Marking);
		GetOwner()->Destroy();
	}

	LatestEntityStatePDUTimestamp = FDateTime::Now();
	MostRecentEntityStatePDU = NewEntityStatePDU;
	DeadReckoningEntityStatePDU = MostRecentEntityStatePDU;

	EntityType = NewEntityStatePDU->EntityStatePDUStruct.EntityType;
	EntityID = NewEntityStatePDU->EntityStatePDUStruct.EntityID;

	GetOwner()->SetLifeSpan(DISTimeoutSeconds);

	OnReceivedEntityStatePDU.Broadcast(NewEntityStatePDU);

	GroundClamping_Implementation();

	//If tick is not enabled and dead reckoning is enabled, then enable tick
	//Tick is off by default to prevent race conditions since Dead Reckoning relies on the Entity State PDU
	if (!this->IsComponentTickEnabled() && PerformDeadReckoning)
	{
		this->SetComponentTickEnabled(true);
	}
}

void UDISComponent::HandleEntityStateUpdatePDU(UGRILL_EntityStateUpdatePDU* NewEntityStateUpdatePDU)
{
	//Check if the entity has been deactivated -- Entity is deactivated if the 23rd bit of the Entity Appearance value is set
	if (NewEntityStateUpdatePDU->EntityStateUpdatePDUStruct.EntityAppearance & (1 << 23))
	{
		UE_LOG(LogDISComponent, Log, TEXT("%s Entity Appearance is set to deactivated, deleting entity..."), *NewEntityStateUpdatePDU->EntityStateUpdatePDUStruct.EntityID.ToString());
		GetOwner()->Destroy();
	}

	LatestEntityStatePDUTimestamp = FDateTime::Now();
	MostRecentEntityStatePDU->SetupFromEntityStateUpdatePDU(NewEntityStateUpdatePDU);
	DeadReckoningEntityStatePDU = MostRecentEntityStatePDU;

	EntityID = NewEntityStateUpdatePDU->EntityStateUpdatePDUStruct.EntityID;

	GetOwner()->SetLifeSpan(DISTimeoutSeconds);

	OnReceivedEntityStateUpdatePDU.Broadcast(NewEntityStateUpdatePDU);

	GroundClamping_Implementation();

	//If tick is not enabled and dead reckoning is enabled, then enable tick
	//Tick is off by default to prevent race conditions since Dead Reckoning relies on the Entity State PDU
	if (!this->IsComponentTickEnabled() && PerformDeadReckoning)
	{
		this->SetComponentTickEnabled(true);
	}
}

void UDISComponent::HandleFirePDU(UGRILL_FirePDU* FirePDUIn)
{
	OnReceivedFirePDU.Broadcast(FirePDUIn);
}

void UDISComponent::HandleDetonationPDU(UGRILL_DetonationPDU* DetonationPDUIn)
{
	OnReceivedDetonationPDU.Broadcast(DetonationPDUIn);
}

void UDISComponent::HandleRemoveEntityPDU(UGRILL_RemoveEntityPDU* RemoveEntityPDUIn)
{
	OnReceivedRemoveEntityPDU.Broadcast(RemoveEntityPDUIn);
}

// TODO: Cleanup copy pasted code in switch
bool UDISComponent::DeadReckoning(UGRILL_EntityStatePDU* EntityPDUToDeadReckon, float DeltaTime, UGRILL_EntityStatePDU*& DeadReckonedEntityPDU)
{
	//Check if dead reckoning should be performed and if the entity is owned by another sim on the network
	//If not, then don't do dead reckoning
	if (!PerformDeadReckoning || !SpawnedFromNetwork)
	{
		PerformDeadReckoning = false;
		return false;
	}

	//Check if the Entity to Dead Reckon is valid. If not, don't try to perform dead reckoning on it
	if (!IsValid(EntityPDUToDeadReckon))
	{
		UE_LOG(LogDISComponent, Error, TEXT("Entity PDU passed to Dead Reckon is not valid. Exiting Dead Reckoning early."));
		return false;
	}
	
	DeadReckonedEntityPDU = EntityPDUToDeadReckon;
	bool bSupported = true;

	switch (EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.DeadReckoningAlgorithm) {
	case 1: // Static
		{
			FRotator LocalRotator;
			bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, LocalRotator);

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = LocalRotator;
			}

			break;
		}

	case 2: // Fixed Position World (FPW)
		{
			// Set entity orientation
			FRotator LocalRotator;
			bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, LocalRotator);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, glm::dvec3(0), DeltaTime);			

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = LocalRotator;
			}
			break;
		}
	
	case 3: // Rotation Position World (RPW)
		{
			FQuat EntityRotation;
			bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, EntityRotation);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, glm::dvec3(0), DeltaTime);

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Z = CalculatedPositionVector[2];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(EntityRotation);
			} else
			{
				glm::dvec3 AngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.X, 
					EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Z);
				//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
				double PsiRadians, ThetaRadians, PhiRadians;
				CalculateDeadReckonedOrientation(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll, 
					EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, AngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
			}
			break;
		}
	
	case 4: // Rotation Velocity World (RVW)
		{
			FQuat EntityRotation;
			bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, EntityRotation);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);
			glm::dvec3 AccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.X,
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, AccelerationVector, DeltaTime);

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Z = CalculatedPositionVector[2];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(EntityRotation);
			} else
			{
				glm::dvec3 AngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.X, 
					EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Z);
				//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
				double PsiRadians, ThetaRadians, PhiRadians;
				CalculateDeadReckonedOrientation(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch, 
					EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll, AngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
			}
			break;
		}
	
	case 5: // Fixed Velocity World (FVW)
		{
			FRotator LocalRotator;
			bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, LocalRotator);

			// Calculate and set entity location
			glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);
			glm::dvec3 AccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.X, 
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Z);

			auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, AccelerationVector, DeltaTime);

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Z = CalculatedPositionVector[2];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = LocalRotator;
			}
			break;
		}

	case 6: // Fixed Position Body (FPB)
		{
			FRotator LocalRotator;
			bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, LocalRotator);

			auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);
			auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.X,
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Z);
			auto BodyAngularAccelerationVector = glm::dvec3(0);
			auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll);

			glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
				BodyAngularAccelerationVector, EntityOrientation, DeltaTime);

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Z = CalculatedPositionVector[2];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = LocalRotator;
			}
			
			break;
		}

	case 7: // Rotation Position Body (RPB)
		{
			FQuat EntityRotation;
			bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, EntityRotation);

			auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);
			auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.X,
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Z);
			auto BodyAngularAccelerationVector = glm::dvec3(0);
			auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll);

			glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
				BodyAngularAccelerationVector, EntityOrientation, DeltaTime);

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Z = CalculatedPositionVector[2];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(EntityRotation);
			} else
			{
				//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
				double PsiRadians, ThetaRadians, PhiRadians;
				CalculateDeadReckonedOrientation(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch,
					EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll, BodyAngularAccelerationVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
			}
			
			break;
		}

	case 8: // Rotation Velocity Body (RVB)
		{
			FQuat EntityRotation;
			bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, EntityRotation);

			auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);
			auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.X, 
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Z);
			auto BodyAngularAccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.X, 
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Z);
			auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll);

			glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
			                                  BodyAngularAccelerationVector, EntityOrientation, DeltaTime);

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Z = CalculatedPositionVector[2];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(EntityRotation);
			} else
			{
				//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
				double PsiRadians, ThetaRadians, PhiRadians;
				CalculateDeadReckonedOrientation(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch,
					EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll, BodyAngularAccelerationVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
			}
			
			break;
		}

	case 9: // Fixed Velocity Body (FVB)
		{
			FRotator LocalRotator;
			bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.OtherParameters, LocalRotator);

			auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[0], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[1], EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLocationDouble[2]);
			auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.X, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityLinearVelocity.Z);
			auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.X,
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration.Z);
			auto BodyAngularAccelerationVector = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.X,
				EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon->EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity.Z);
			auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Yaw, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Pitch, EntityPDUToDeadReckon->EntityStatePDUStruct.EntityOrientation.Roll);

			glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
				BodyAngularAccelerationVector, EntityOrientation, DeltaTime);

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[0] = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[1] = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocationDouble[2] = CalculatedPositionVector[2];

			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.X = CalculatedPositionVector[0];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Y = CalculatedPositionVector[1];
			DeadReckonedEntityPDU->EntityStatePDUStruct.EntityLocation.Z = CalculatedPositionVector[2];

			if (bUseOtherParams)
			{
				DeadReckonedEntityPDU->EntityStatePDUStruct.EntityOrientation = LocalRotator;
			}
			
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

void UDISComponent::GroundClamping_Implementation()
{
	//Verify that ground clamping is enabled, the entity is owned by another sim, is of the ground domain, and that it is not a munition
	if (PerformGroundClamping && SpawnedFromNetwork && EntityType.Domain == 1 && EntityType.EntityKind != 2)
	{
		//If the PDU we're going to ground clamp is not valid, don't try to use it
		if (!IsValid(DeadReckoningEntityStatePDU)) 
		{
			return;
		}

		//Get the most recent calculated ECEF location of the entity from the dead reckoned ESPDU
		FEarthCenteredEarthFixedDouble ecefDouble = FEarthCenteredEarthFixedDouble(DeadReckoningEntityStatePDU->EntityStatePDUStruct.EntityLocationDouble[0],
			DeadReckoningEntityStatePDU->EntityStatePDUStruct.EntityLocationDouble[1], DeadReckoningEntityStatePDU->EntityStatePDUStruct.EntityLocationDouble[2]);

		//Get the LLH location of the entity from the ECEF location
		FLatLonHeightDouble llhDouble;
		UDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(ecefDouble, llhDouble);

		//Get the North East Down vectors from the calculated LLH
		FNorthEastDown northEastDownVectors;
		UDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(llhDouble.Latitude, llhDouble.Longitude, northEastDownVectors);
		//Set clamp direction using the North East Down down vector
		FVector clampDirection = northEastDownVectors.DownVector;

		//Get the location the object is supposed to be at according to the most recent dead reckoning update.
		FVector actorLocation;
		UDIS_BPFL::GetEntityUnrealLocationFromEntityStatePdu(DeadReckoningEntityStatePDU, AGeoReferencingSystem::GetGeoReferencingSystem(GetWorld()), actorLocation);

		FHitResult lineTraceHitResult;
		FVector endLocation = (clampDirection * 100000) + actorLocation;
		FVector aboveActorStartLocation = (clampDirection * -100000) + actorLocation;

		FCollisionQueryParams queryParams = FCollisionQueryParams(FName("Ground Clamping"), false, GetOwner());
		//Find colliding point above/below the actor
		if (GetWorld()->LineTraceSingleByChannel(lineTraceHitResult, aboveActorStartLocation, endLocation, UEngineTypes::ConvertToCollisionChannel(GoundClampingCollisionChannel), queryParams))
		{
			FVector clampLocation = lineTraceHitResult.Location;
			//Calculate what the new forward and right vectors should be based on the impact normal
			FVector newForward = FVector::CrossProduct(GetOwner()->GetActorRightVector(), lineTraceHitResult.ImpactNormal);
			FVector newRight = FVector::CrossProduct(lineTraceHitResult.ImpactNormal, newForward);

			FRotator clampRotation = UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, lineTraceHitResult.ImpactNormal);

			//Create clamp transform and broadcast
			FTransform clampTransform = FTransform(clampRotation, clampLocation);
			TArray<FTransform> allClampTransforms;
			allClampTransforms.Add(clampTransform);

			OnGroundClampingUpdate.Broadcast(allClampTransforms);
		}
	}
}