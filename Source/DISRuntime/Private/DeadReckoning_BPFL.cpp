// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.


#include "DeadReckoning_BPFL.h"
#include "Algo/Reverse.h"

const double UDeadReckoning_BPFL::MIN_ROTATION_RATE = 0.2 * glm::pi<double>() / 180;  // minimum significant rate = 1deg/5sec

bool UDeadReckoning_BPFL::IsMachineLittleEndian()
{
	//Check the endianness of the machine
	long one(1);
	char e = (reinterpret_cast<char*>(&one))[0];

	return (e == (char)1);
}

FRotator UDeadReckoning_BPFL::CalculateDeadReckonedEulerAnglesFromQuaternion(FEntityStatePDU EntityPDUToDeadReckon, FQuat EntityRotationQuaternion, float DeltaTime)
{
	//Calculate dead reckoning orienation quaternion
	glm::dvec3 AngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.X,
		EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Z);
	FQuat deadReckonedQuaternion = CreateDeadReckoningQuaternion(AngularVelocityVector, DeltaTime);
	FQuat finalDeadReckonedQuat = EntityRotationQuaternion * deadReckonedQuaternion;

	//Convert quaternion to Psi, Thet, Phi
	float psi = FMath::Atan2(2 * (finalDeadReckonedQuat.X * finalDeadReckonedQuat.Y + finalDeadReckonedQuat.W * finalDeadReckonedQuat.Z),
		(FMath::Square(finalDeadReckonedQuat.W) + FMath::Square(finalDeadReckonedQuat.X) - FMath::Square(finalDeadReckonedQuat.Y) - FMath::Square(finalDeadReckonedQuat.Z)));
	float theta = FMath::Asin(-2 * (finalDeadReckonedQuat.X * finalDeadReckonedQuat.Z - finalDeadReckonedQuat.W * finalDeadReckonedQuat.Y));
	float phi = FMath::Atan2(2 * (finalDeadReckonedQuat.Y * finalDeadReckonedQuat.Z + finalDeadReckonedQuat.W * finalDeadReckonedQuat.X),
		(FMath::Square(finalDeadReckonedQuat.W) - FMath::Square(finalDeadReckonedQuat.X) - FMath::Square(finalDeadReckonedQuat.Y) + FMath::Square(finalDeadReckonedQuat.Z)));

	if (theta == glm::pi<float>() / 2)
	{
		theta = 1e-5;
	}

	return FRotator(theta, psi, phi);
}

FRotator UDeadReckoning_BPFL::CalculateDirectionalRotationDifference(FRotator OldRotation, FRotator NewRotation)
{
	//Convert the rotators to quaternions
	FQuat oldQuat = OldRotation.Quaternion();
	FQuat newQuat = NewRotation.Quaternion();
	oldQuat.Normalize();
	newQuat.Normalize();

	//Get the rotational difference between the quaternions -- Gives back direction of rotation too
	FQuat rotDiff = newQuat * oldQuat.Inverse();

	return rotDiff.Rotator();
}

TArray<uint8> UDeadReckoning_BPFL::FormOtherParameters(EDeadReckoningAlgorithm DeadReckoningAlgorithm, FRotator EntityPsiThetaPhiRadians, FVector EntityECEFLocation)
{
	TArray<uint8> otherParameters;
	otherParameters.Init(0, 15);

	unsigned char charHeading[sizeof(float)];
	unsigned char charPitch[sizeof(float)];
	unsigned char charRoll[sizeof(float)];

	if (DeadReckoningAlgorithm == EDeadReckoningAlgorithm::Static || DeadReckoningAlgorithm == EDeadReckoningAlgorithm::FPW || DeadReckoningAlgorithm == EDeadReckoningAlgorithm::FVW 
		|| DeadReckoningAlgorithm == EDeadReckoningAlgorithm::FPB || DeadReckoningAlgorithm == EDeadReckoningAlgorithm::FVB)
	{
		otherParameters[0] = 1;
		//Padding: Unused
		otherParameters[1] = 0;
		otherParameters[2] = 0;

;		FLatLonHeightFloat llh;
		FHeadingPitchRoll hprRadians;
		FEarthCenteredEarthFixedFloat ecef = FEarthCenteredEarthFixedFloat(EntityECEFLocation.X, EntityECEFLocation.Y, EntityECEFLocation.Z);
		FPsiThetaPhi psiThetaPhiRadians = FPsiThetaPhi(EntityPsiThetaPhiRadians);
		UDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(ecef, llh);
		UDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, llh.Latitude, llh.Longitude, hprRadians);

		//Convert the floats to unsigned char arrays
		memcpy(charHeading, &hprRadians.Heading, sizeof(hprRadians.Heading));
		memcpy(charPitch, &hprRadians.Pitch, sizeof(hprRadians.Pitch));
		memcpy(charRoll, &hprRadians.Roll, sizeof(hprRadians.Roll));

		if (IsMachineLittleEndian())
		{
			Algo::Reverse(charHeading);
			Algo::Reverse(charPitch);
			Algo::Reverse(charRoll);
		}
	}
	else if (DeadReckoningAlgorithm == EDeadReckoningAlgorithm::RPW || DeadReckoningAlgorithm == EDeadReckoningAlgorithm::RVW || DeadReckoningAlgorithm == EDeadReckoningAlgorithm::RPB || DeadReckoningAlgorithm == EDeadReckoningAlgorithm::RVB)
	{
		otherParameters[0] = 2;

		//Calculate the four quaternion terms
		float cosPsiHalved = glm::cos(EntityPsiThetaPhiRadians.Yaw / 2);
		float cosThetaHalved = glm::cos(EntityPsiThetaPhiRadians.Pitch / 2);
		float cosPhiHalved = glm::cos(EntityPsiThetaPhiRadians.Roll / 2);
		float sinPsiHalved = glm::sin(EntityPsiThetaPhiRadians.Yaw / 2);
		float sinThetaHalved = glm::sin(EntityPsiThetaPhiRadians.Pitch / 2);
		float sinPhiHalved = glm::sin(EntityPsiThetaPhiRadians.Roll / 2);

		float qu0 = cosPsiHalved * cosThetaHalved * cosPhiHalved + sinPsiHalved * sinThetaHalved * sinPhiHalved;
		float qux = cosPsiHalved * cosThetaHalved * sinPhiHalved - sinPsiHalved * sinThetaHalved * cosPhiHalved;
		float quy = cosPsiHalved * sinThetaHalved * cosPhiHalved + sinPsiHalved * cosThetaHalved * sinPhiHalved;
		float quz = sinPsiHalved * cosThetaHalved * cosPhiHalved - cosPsiHalved * sinThetaHalved * sinPhiHalved;

		//If qu0 is negative, invert all terms
		if (qu0 < 0)
		{
			qu0 *= -1;
			qux *= -1;
			quy *= -1;
			quz *= -1;
		}
		//qu0 could be greater than 0 due to rounding error. Form finalQu0 based on this.
		uint16 finalQu0 = (qu0 >= 1) ? 65535 : FMath::TruncToInt(qu0 * 65536);
		//Convert the floats to unsigned char arrays
		memcpy(charHeading, &qux, sizeof(qux));
		memcpy(charPitch, &quy, sizeof(quy));
		memcpy(charRoll, &quz, sizeof(quz));

		if (IsMachineLittleEndian())
		{
			Algo::Reverse(charHeading);
			Algo::Reverse(charPitch);
			Algo::Reverse(charRoll);
		}

		//Fill out Qu0 portion of Other Parameters
		otherParameters[1] = finalQu0 >> 8;
		otherParameters[2] = finalQu0;
	}
	else
	{
		//Unknown Dead Reckoning algorithm being used
		return otherParameters;
	}

	//Actually fill out the other parameters
	otherParameters[3] = charHeading[0];
	otherParameters[4] = charHeading[1];
	otherParameters[5] = charHeading[2];
	otherParameters[6] = charHeading[3];
	otherParameters[7] = charPitch[0];
	otherParameters[8] = charPitch[1];
	otherParameters[9] = charPitch[2];
	otherParameters[10] = charPitch[3];
	otherParameters[11] = charRoll[0];
	otherParameters[12] = charRoll[1];
	otherParameters[13] = charRoll[2];
	otherParameters[14] = charRoll[3];

	return otherParameters;
}

bool UDeadReckoning_BPFL::GetLocalEulerAngles(TArray<uint8> OtherDeadReckoningParameters, FRotator& LocalRotator)
{
	// Ensure the Array is at least 15 bytes long
	if (OtherDeadReckoningParameters.Num() < 15) return false;

	// Ensure the DR Parameter type is set to 1
	if (OtherDeadReckoningParameters[0] != 1) return false;

	// Skip indices 1 and 2 as they are padding and unused
	// The heading, pitch, and roll are the next three groups of four bytes respectively
	unsigned char charHeading[sizeof(float)] = { OtherDeadReckoningParameters[3], OtherDeadReckoningParameters[4], OtherDeadReckoningParameters[5], OtherDeadReckoningParameters[6] };
	unsigned char charPitch[sizeof(float)] = { OtherDeadReckoningParameters[7], OtherDeadReckoningParameters[8], OtherDeadReckoningParameters[9], OtherDeadReckoningParameters[10] };
	unsigned char charRoll[sizeof(float)] = { OtherDeadReckoningParameters[11], OtherDeadReckoningParameters[12], OtherDeadReckoningParameters[13], OtherDeadReckoningParameters[14] };

	if (IsMachineLittleEndian())
	{
		Algo::Reverse(charHeading);
		Algo::Reverse(charPitch);
		Algo::Reverse(charRoll);
	}

	// Memcpy chars into floats
	float LocalYaw;
	memcpy(&LocalYaw, charHeading, sizeof(float));
	float LocalPitch;
	memcpy(&LocalPitch, charPitch, sizeof(float));
	float LocalRoll;
	memcpy(&LocalRoll, charRoll, sizeof(float));

	// Set the values and return
	LocalRotator = FRotator(LocalPitch, LocalYaw, LocalRoll);

	return true;
}

void UDeadReckoning_BPFL::ConvertLocalRotatorToPsiThetaPhiRadians(FEntityStatePDU EntityPDUToDeadReckon, FRotator LocalRotatorRadians, FPsiThetaPhi& PsiThetaPhiRadians)
{
	//Convert Local Rotator from Heading, Pitch, Roll to Psi, Theta, Phi
	FEarthCenteredEarthFixedDouble ecef = FEarthCenteredEarthFixedDouble(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
	FLatLonHeightDouble llh;
	UDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(ecef, llh);

	FHeadingPitchRoll hprRadians = FHeadingPitchRoll(LocalRotatorRadians.Yaw, LocalRotatorRadians.Pitch, LocalRotatorRadians.Roll);
	UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(hprRadians, llh.Latitude, llh.Longitude, PsiThetaPhiRadians);
}

bool UDeadReckoning_BPFL::GetLocalQuaternionAngles(TArray<uint8> OtherDeadReckoningParameters, FQuat& EntityOrientation)
{
	// Ensure the array is at least 15 bytes long
	if (OtherDeadReckoningParameters.Num() < 15) return false;

	// Ensure the DR Parameter type is set to 2
	if (OtherDeadReckoningParameters[0] != 2) return false;

	//The next two bytes represent the 16 bit unsigned int approximation of q_0 (q_w in UE4 terminology)
	uint16 Qu0 = (OtherDeadReckoningParameters[1] << 8) + OtherDeadReckoningParameters[2];

	// The quaternion x, y, and z components are the next three groups of four bytes respectively
	unsigned char charQuX[sizeof(float)] = { OtherDeadReckoningParameters[3], OtherDeadReckoningParameters[4], OtherDeadReckoningParameters[5], OtherDeadReckoningParameters[6] };
	unsigned char charQuY[sizeof(float)] = { OtherDeadReckoningParameters[7], OtherDeadReckoningParameters[8], OtherDeadReckoningParameters[9], OtherDeadReckoningParameters[10] };
	unsigned char charQuZ[sizeof(float)] = { OtherDeadReckoningParameters[11], OtherDeadReckoningParameters[12], OtherDeadReckoningParameters[13], OtherDeadReckoningParameters[14] };

	if (IsMachineLittleEndian())
	{
		Algo::Reverse(charQuX);
		Algo::Reverse(charQuY);
		Algo::Reverse(charQuZ);
	}

	// Memcpy chars into floats
	float QuX;
	memcpy(&QuX, charQuX, sizeof(float));
	float QuY;
	memcpy(&QuY, charQuY, sizeof(float));
	float QuZ;
	memcpy(&QuZ, charQuZ, sizeof(float));

	// Calculate the appropriate Qu0
	float finalQu0 = FMath::Sqrt(1 - (FMath::Square(QuX) + FMath::Square(QuY) + FMath::Square(QuZ)));

	// Set the values and return
	EntityOrientation = FQuat(QuX, QuY, QuZ, finalQu0);

	return true;
}

glm::dvec3 UDeadReckoning_BPFL::CalculateDeadReckonedPosition(const glm::dvec3 PositionVector, const glm::dvec3 VelocityVector,
	const glm::dvec3 AccelerationVector, const double DeltaTime)
{
	return PositionVector + (VelocityVector * DeltaTime) + (0.5 * AccelerationVector * FMath::Square(DeltaTime));
}

glm::dmat3 UDeadReckoning_BPFL::CreateDeadReckoningMatrix(glm::dvec3 AngularVelocityVector, double DeltaTime)
{
	double AngularVelocityMagnitude = glm::length(AngularVelocityVector);
	if (AngularVelocityMagnitude == 0)
	{
		AngularVelocityMagnitude = 1e-5;
		AngularVelocityVector += glm::dvec3(1e-5);
	}

	const auto AngularVelocityMatrix = glm::dmat3(AngularVelocityVector, glm::dvec3(0), glm::dvec3(0));
	const auto AngularVelocity = AngularVelocityMatrix * glm::transpose(AngularVelocityMatrix);

	const auto CosOmega = glm::cos(AngularVelocityMagnitude * DeltaTime);
	const auto SinOmega = glm::sin(AngularVelocityMagnitude * DeltaTime);

	glm::dmat3 DeadReckoningMatrix = (((1 - CosOmega) / glm::pow(AngularVelocityMagnitude, 2)) * AngularVelocity) +
		(CosOmega * glm::dmat3(1)) -
		(SinOmega / AngularVelocityMagnitude * UDIS_BPFL::CreateNCrossXMatrix(AngularVelocityVector));

	return DeadReckoningMatrix;
}

FQuat UDeadReckoning_BPFL::CreateDeadReckoningQuaternion(glm::dvec3 AngularVelocityVector, double DeltaTime)
{
	double AngularVelocityMagnitude = glm::length(AngularVelocityVector);
	if (AngularVelocityMagnitude == 0)
	{
		AngularVelocityMagnitude = 1e-5;
		AngularVelocityVector += glm::dvec3(1e-5);
	}

	double beta = AngularVelocityMagnitude * DeltaTime;
	glm::dvec3 unitVector = AngularVelocityVector / AngularVelocityMagnitude;

	FQuat deadReckoningQuaternion = FQuat();

	deadReckoningQuaternion.W = glm::cos(beta / 2);
	deadReckoningQuaternion.X = unitVector.x * glm::sin(beta / 2);
	deadReckoningQuaternion.Y = unitVector.y * glm::sin(beta / 2);
	deadReckoningQuaternion.Z = unitVector.z * glm::sin(beta / 2);

	return deadReckoningQuaternion;
}

glm::dmat3 UDeadReckoning_BPFL::GetEntityOrientationMatrix(const double PsiRadians, const double ThetaRadians, const double PhiRadians)
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

FQuat UDeadReckoning_BPFL::GetEntityOrientationQuaternion(double PsiRadians, double ThetaRadians, double PhiRadians)
{
	//Abbreviations for the various angular functions
	double cy = glm::cos(PsiRadians / 2);
	double sy = glm::sin(PsiRadians / 2);
	double cp = glm::cos(ThetaRadians / 2);
	double sp = glm::sin(ThetaRadians / 2);
	double cr = glm::cos(PhiRadians / 2);
	double sr = glm::sin(PhiRadians / 2);

	FQuat entityQuaternion = FQuat();
	entityQuaternion.W = cr * cp * cy + sr * sp * sy;
	entityQuaternion.X = sr * cp * cy - cr * sp * sy;
	entityQuaternion.Y = cr * sp * cy + sr * cp * sy;
	entityQuaternion.Z = cr * cp * sy - sr * sp * cy;

	return entityQuaternion;
}

void UDeadReckoning_BPFL::CalculateDeadReckonedOrientation(const double PsiRadians, const double ThetaRadians, const double PhiRadians,
	const glm::dvec3 AngularVelocityVector, const float DeltaTime, double& OutPsiRadians, double& OutThetaRadians, double& OutPhiRadians)
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
	if (abs(OutThetaRadians) != glm::pi<double>() / 2)
	{
		CosTheta = glm::cos(OutThetaRadians);
	}

	OutPsiRadians = glm::acos(OrientationMatrix[0][0] / CosTheta) * (abs(OrientationMatrix[1][0]) / OrientationMatrix[1][0]);
	OutPhiRadians = glm::acos(OrientationMatrix[2][2] / CosTheta) * (abs(OrientationMatrix[2][1]) / OrientationMatrix[2][1]);
}

glm::dvec3 UDeadReckoning_BPFL::GetEntityBodyDeadReckonedPosition(const glm::dvec3 InitialPositionVector, const glm::dvec3 BodyVelocityVector, const glm::dvec3 BodyLinearAccelerationVector, const glm::dvec3 BodyAngularVelocityVector, const glm::dvec3 EntityOrientation, const double DeltaTime)
{
	const auto SkewMatrix = UDIS_BPFL::CreateNCrossXMatrix(BodyAngularVelocityVector);
	const auto BodyAccelerationVector = BodyLinearAccelerationVector - (SkewMatrix * BodyVelocityVector);

	// Get the entity's current orientation matrix
	const auto OrientationMatrix = GetEntityOrientationMatrix(EntityOrientation.x, EntityOrientation.y, EntityOrientation.z);
	const auto InverseInitialOrientationMatrix = glm::transpose(OrientationMatrix);

	// Calculate R1
	const auto OmegaMatrix = glm::dmat3x3(BodyAngularVelocityVector, glm::dvec3(0), glm::dvec3(0)) * glm::transpose(glm::dmat3x3(BodyAngularVelocityVector, glm::dvec3(0), glm::dvec3(0)));
	double AngularVelocityMagnitude = glm::length(BodyAngularVelocityVector);

	glm::dmat3 R1 = glm::dmat3();
	glm::dmat3 R2 = glm::dmat3();

	if (AngularVelocityMagnitude < MIN_ROTATION_RATE)
	{
		R1 = glm::dmat3(1) * DeltaTime;
		R2 = glm::dmat3(1) * (glm::pow(DeltaTime, 2) / 2);
	}
	else
	{
		R1 = (((AngularVelocityMagnitude * DeltaTime - glm::sin(AngularVelocityMagnitude * DeltaTime)) / glm::pow(AngularVelocityMagnitude, 3)) * OmegaMatrix) +
			(glm::sin(AngularVelocityMagnitude * DeltaTime) / AngularVelocityMagnitude * glm::dmat3(1)) +
			(((1 - glm::cos(AngularVelocityMagnitude * DeltaTime)) / glm::pow(AngularVelocityMagnitude, 2)) * SkewMatrix);

		R2 = ((((0.5 * glm::pow(AngularVelocityMagnitude, 2) * glm::pow(DeltaTime, 2)) - (glm::cos(AngularVelocityMagnitude * DeltaTime)) - (AngularVelocityMagnitude * DeltaTime * glm::sin(AngularVelocityMagnitude * DeltaTime)) + (1)) / glm::pow(AngularVelocityMagnitude, 4)) * OmegaMatrix) +
			(((glm::cos(AngularVelocityMagnitude * DeltaTime)) + (AngularVelocityMagnitude * DeltaTime * glm::sin(AngularVelocityMagnitude * DeltaTime)) - (1)) / (glm::pow(AngularVelocityMagnitude, 2)) * glm::dmat3(1)) +
			((((glm::sin(AngularVelocityMagnitude * DeltaTime)) - (AngularVelocityMagnitude * DeltaTime * glm::cos(AngularVelocityMagnitude * DeltaTime))) / (glm::pow(AngularVelocityMagnitude, 3))) * SkewMatrix);
	}

	return InitialPositionVector + InverseInitialOrientationMatrix * ((R1 * BodyVelocityVector) + (R2 * BodyAccelerationVector));
}

bool UDeadReckoning_BPFL::DeadReckoning(FEntityStatePDU EntityPDUToDeadReckon, float DeltaTime, FEntityStatePDU& DeadReckonedEntityPDU)
{
	DeadReckonedEntityPDU = EntityPDUToDeadReckon;
	bool bSupported = true;

	//If the entity is frozen, don't update dead reckoning
	if (EntityPDUToDeadReckon.EntityAppearance & (1 << 21))
	{
		return false;
	}

	switch (EntityPDUToDeadReckon.DeadReckoningParameters.DeadReckoningAlgorithm)
	{
	case EDeadReckoningAlgorithm::Static: // Static
	{
		FRotator LocalRotator;
		bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

		if (bUseOtherParams)
		{
			FPsiThetaPhi psiThetaPhiRadians;
			ConvertLocalRotatorToPsiThetaPhiRadians(EntityPDUToDeadReckon, LocalRotator, psiThetaPhiRadians);
			DeadReckonedEntityPDU.EntityOrientation = FRotator(psiThetaPhiRadians.Theta, psiThetaPhiRadians.Psi, psiThetaPhiRadians.Phi);
		}

		break;
	}

	case EDeadReckoningAlgorithm::FPW: // Fixed Position World (FPW)
	{
		// Set entity orientation
		FRotator LocalRotator;
		bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

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

		if (bUseOtherParams)
		{
			FPsiThetaPhi psiThetaPhiRadians;
			ConvertLocalRotatorToPsiThetaPhiRadians(EntityPDUToDeadReckon, LocalRotator, psiThetaPhiRadians);
			DeadReckonedEntityPDU.EntityOrientation = FRotator(psiThetaPhiRadians.Theta, psiThetaPhiRadians.Psi, psiThetaPhiRadians.Phi);
		}
		break;
	}

	case EDeadReckoningAlgorithm::RPW: // Rotation Position World (RPW)
	{
		FQuat EntityRotationQuaternion;
		bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotationQuaternion);

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

		if (bUseOtherParams)
		{
			DeadReckonedEntityPDU.EntityOrientation = CalculateDeadReckonedEulerAnglesFromQuaternion(EntityPDUToDeadReckon, EntityRotationQuaternion, DeltaTime);
		}
		else
		{
			glm::dvec3 AngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.X,
				EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Z);
			//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
			double PsiRadians, ThetaRadians, PhiRadians;
			CalculateDeadReckonedOrientation(EntityPDUToDeadReckon.EntityOrientation.Yaw,
				EntityPDUToDeadReckon.EntityOrientation.Pitch, EntityPDUToDeadReckon.EntityOrientation.Roll, AngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

			DeadReckonedEntityPDU.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
		}
		break;
	}

	case EDeadReckoningAlgorithm::RVW: // Rotation Velocity World (RVW)
	{
		FQuat EntityRotationQuaternion;
		bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotationQuaternion);

		// Calculate and set entity location
		glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
		glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
		glm::dvec3 AccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);

		auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, AccelerationVector, DeltaTime);

		DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

		DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

		if (bUseOtherParams)
		{
			DeadReckonedEntityPDU.EntityOrientation = CalculateDeadReckonedEulerAnglesFromQuaternion(EntityPDUToDeadReckon, EntityRotationQuaternion, DeltaTime);
		}
		else
		{
			glm::dvec3 AngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.X,
				EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Z);
			//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
			double PsiRadians, ThetaRadians, PhiRadians;
			CalculateDeadReckonedOrientation(EntityPDUToDeadReckon.EntityOrientation.Yaw, EntityPDUToDeadReckon.EntityOrientation.Pitch,
				EntityPDUToDeadReckon.EntityOrientation.Roll, AngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

			DeadReckonedEntityPDU.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
		}
		break;
	}

	case EDeadReckoningAlgorithm::FVW: // Fixed Velocity World (FVW)
	{
		FRotator LocalRotator;
		bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

		// Calculate and set entity location
		glm::dvec3 PositionVector = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
		glm::dvec3 VelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
		glm::dvec3 AccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);

		auto CalculatedPositionVector = CalculateDeadReckonedPosition(PositionVector, VelocityVector, AccelerationVector, DeltaTime);

		DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

		DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

		if (bUseOtherParams)
		{
			FPsiThetaPhi psiThetaPhiRadians;
			ConvertLocalRotatorToPsiThetaPhiRadians(EntityPDUToDeadReckon, LocalRotator, psiThetaPhiRadians);
			DeadReckonedEntityPDU.EntityOrientation = FRotator(psiThetaPhiRadians.Theta, psiThetaPhiRadians.Psi, psiThetaPhiRadians.Phi);
		}
		break;
	}

	case EDeadReckoningAlgorithm::FPB: // Fixed Position Body (FPB)
	{
		FRotator LocalRotator;
		bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

		auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
		auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
		auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);
		auto BodyAngularVelocityVector = glm::dvec3(0);
		auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon.EntityOrientation.Yaw, EntityPDUToDeadReckon.EntityOrientation.Pitch, EntityPDUToDeadReckon.EntityOrientation.Roll);

		glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
			BodyAngularVelocityVector, EntityOrientation, DeltaTime);

		DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

		DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

		if (bUseOtherParams)
		{
			FPsiThetaPhi psiThetaPhiRadians;
			ConvertLocalRotatorToPsiThetaPhiRadians(EntityPDUToDeadReckon, LocalRotator, psiThetaPhiRadians);
			DeadReckonedEntityPDU.EntityOrientation = FRotator(psiThetaPhiRadians.Theta, psiThetaPhiRadians.Psi, psiThetaPhiRadians.Phi);
		}

		break;
	}

	case EDeadReckoningAlgorithm::RPB: // Rotation Position Body (RPB)
	{
		FQuat EntityRotationQuaternion;
		bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotationQuaternion);

		auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
		auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
		auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);
		auto BodyAngularVelocityVector = glm::dvec3(0);
		auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon.EntityOrientation.Yaw, EntityPDUToDeadReckon.EntityOrientation.Pitch, EntityPDUToDeadReckon.EntityOrientation.Roll);

		glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
			BodyAngularVelocityVector, EntityOrientation, DeltaTime);

		DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

		DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

		if (bUseOtherParams)
		{
			DeadReckonedEntityPDU.EntityOrientation = CalculateDeadReckonedEulerAnglesFromQuaternion(EntityPDUToDeadReckon, EntityRotationQuaternion, DeltaTime);
		}
		else
		{
			//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
			double PsiRadians, ThetaRadians, PhiRadians;
			CalculateDeadReckonedOrientation(EntityPDUToDeadReckon.EntityOrientation.Yaw, EntityPDUToDeadReckon.EntityOrientation.Pitch,
				EntityPDUToDeadReckon.EntityOrientation.Roll, BodyAngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

			DeadReckonedEntityPDU.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
		}

		break;
	}

	case EDeadReckoningAlgorithm::RVB: // Rotation Velocity Body (RVB)
	{
		FQuat EntityRotationQuaternion;
		bool bUseOtherParams = GetLocalQuaternionAngles(DeadReckonedEntityPDU.DeadReckoningParameters.OtherParameters, EntityRotationQuaternion);

		auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
		auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
		auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);
		auto BodyAngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Z);
		auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon.EntityOrientation.Yaw, EntityPDUToDeadReckon.EntityOrientation.Pitch, EntityPDUToDeadReckon.EntityOrientation.Roll);

		glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
			BodyAngularVelocityVector, EntityOrientation, DeltaTime);

		DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

		DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

		if (bUseOtherParams)
		{
			DeadReckonedEntityPDU.EntityOrientation = CalculateDeadReckonedEulerAnglesFromQuaternion(EntityPDUToDeadReckon, EntityRotationQuaternion, DeltaTime);
		}
		else
		{
			//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
			double PsiRadians, ThetaRadians, PhiRadians;
			CalculateDeadReckonedOrientation(EntityPDUToDeadReckon.EntityOrientation.Yaw, EntityPDUToDeadReckon.EntityOrientation.Pitch,
				EntityPDUToDeadReckon.EntityOrientation.Roll, BodyAngularVelocityVector, DeltaTime, PsiRadians, ThetaRadians, PhiRadians);

			DeadReckonedEntityPDU.EntityOrientation = FRotator(ThetaRadians, PsiRadians, PhiRadians);
		}

		break;
	}

	case EDeadReckoningAlgorithm::FVB: // Fixed Velocity Body (FVB)
	{
		FRotator LocalRotator;
		bool bUseOtherParams = GetLocalEulerAngles(EntityPDUToDeadReckon.DeadReckoningParameters.OtherParameters, LocalRotator);

		auto InitialPosition = glm::dvec3(EntityPDUToDeadReckon.EntityLocationDouble[0], EntityPDUToDeadReckon.EntityLocationDouble[1], EntityPDUToDeadReckon.EntityLocationDouble[2]);
		auto BodyVelocityVector = glm::dvec3(EntityPDUToDeadReckon.EntityLinearVelocity.X, EntityPDUToDeadReckon.EntityLinearVelocity.Y, EntityPDUToDeadReckon.EntityLinearVelocity.Z);
		auto BodyLinearAccelerationVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration.Z);
		auto BodyAngularVelocityVector = glm::dvec3(EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.X,
			EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Y, EntityPDUToDeadReckon.DeadReckoningParameters.EntityAngularVelocity.Z);
		auto EntityOrientation = glm::dvec3(EntityPDUToDeadReckon.EntityOrientation.Yaw, EntityPDUToDeadReckon.EntityOrientation.Pitch, EntityPDUToDeadReckon.EntityOrientation.Roll);

		glm::dvec3 CalculatedPositionVector = GetEntityBodyDeadReckonedPosition(InitialPosition, BodyVelocityVector, BodyLinearAccelerationVector,
			BodyAngularVelocityVector, EntityOrientation, DeltaTime);

		DeadReckonedEntityPDU.EntityLocationDouble[0] = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocationDouble[1] = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocationDouble[2] = CalculatedPositionVector[2];

		DeadReckonedEntityPDU.EntityLocation.X = CalculatedPositionVector[0];
		DeadReckonedEntityPDU.EntityLocation.Y = CalculatedPositionVector[1];
		DeadReckonedEntityPDU.EntityLocation.Z = CalculatedPositionVector[2];

		if (bUseOtherParams)
		{
			FPsiThetaPhi psiThetaPhiRadians;
			ConvertLocalRotatorToPsiThetaPhiRadians(EntityPDUToDeadReckon, LocalRotator, psiThetaPhiRadians);
			DeadReckonedEntityPDU.EntityOrientation = FRotator(psiThetaPhiRadians.Theta, psiThetaPhiRadians.Psi, psiThetaPhiRadians.Phi);
		}

		break;
	}

	default: // Unknown
	{
		bSupported = false;
		break;
	}
	}

	if (bSupported)
	{
		DeadReckonedEntityPDU.EntityLinearVelocity += EntityPDUToDeadReckon.DeadReckoningParameters.EntityLinearAcceleration * DeltaTime;
	}

	return bSupported;
}
