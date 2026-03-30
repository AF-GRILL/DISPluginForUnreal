// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.


#include "DIS_BPFL.h"
#include "DISGameManager.h"

DEFINE_LOG_CATEGORY(LogDIS_BPFL);

FString UDIS_BPFL::GetCurrentPlatformName()
{
#if PLATFORM_WINDOWS
	return TEXT("Windows");
#elif PLATFORM_MAC
	return TEXT("Mac");
#else
	return FPlatformProperties::PlatformName();
#endif
}

bool UDIS_BPFL::IsRunningOnWindows()
{
#if PLATFORM_WINDOWS
	return true;
#else
	return false;
#endif
}

bool UDIS_BPFL::IsRunningOnMac()
{
#if PLATFORM_MAC
	return true;
#else
	return false;
#endif
}

bool UDIS_BPFL::CalculateLatLonAltitudeFromEcefXYZ(const FVector Ecef, FGeographicCoordinates& OutLatLonAltDegreesMeters)
{
	constexpr double earthEquitorialRadiusMeters = 6378137;
	constexpr double earthPolarRadiusMeters = 6356752.3142;

	constexpr double earthEquitorialRadiusMetersSquared = FMath::Square(earthEquitorialRadiusMeters);
	constexpr double earthPolarRadiusMetersSquared = FMath::Square(earthPolarRadiusMeters);

	double eSquared = (earthEquitorialRadiusMetersSquared - earthPolarRadiusMetersSquared) / earthEquitorialRadiusMetersSquared;
	double ePrimeSquared = (earthEquitorialRadiusMetersSquared - earthPolarRadiusMetersSquared) / earthPolarRadiusMetersSquared;

	double p = FMath::Sqrt(FMath::Square(Ecef.X) + FMath::Square(Ecef.Y));
	double F = 54 * earthPolarRadiusMetersSquared * FMath::Square(Ecef.Z);
	double G = FMath::Square(p) + (1 - eSquared) * FMath::Square(Ecef.Z) - eSquared * (earthEquitorialRadiusMetersSquared - earthPolarRadiusMetersSquared);
	double c = (FMath::Square(eSquared) * F * FMath::Square(p)) / FMath::Pow(G, 3);

	double s = FMath::Pow(1 + c + FMath::Sqrt(FMath::Square(c) + 2 * c), 1. / 3.);

	if (isnan(s))
	{
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid calculation when converting ECEF to LLA! Given ECEF coordinates resulted in NaN calculation. Returning 0, 0, 0."));
		return false;
	}

	double k = s + 1 + 1 / s;
	double P = F / (3 * FMath::Square(k) * FMath::Square(G));
	double Q = FMath::Sqrt(1 + 2 * FMath::Square(eSquared) * P);

	double rNot = (-P * eSquared * p) / (1 + Q) + FMath::Sqrt(1. / 2. * earthEquitorialRadiusMetersSquared * (1 + 1 / Q) - (P * (1 - eSquared) * FMath::Square(Ecef.Z)) / (Q * (1 + Q)) - 1. / 2. * P * FMath::Square(p));
	double U = FMath::Sqrt(FMath::Square(p - eSquared * rNot) + FMath::Square(Ecef.Z));
	double V = FMath::Sqrt(FMath::Square(p - eSquared * rNot) + (1 - eSquared) * FMath::Square(Ecef.Z));
	double zNot = (earthPolarRadiusMetersSquared * Ecef.Z) / (earthEquitorialRadiusMeters * V);

	OutLatLonAltDegreesMeters.Altitude = U * (1 - earthPolarRadiusMetersSquared / (earthEquitorialRadiusMeters * V));
	OutLatLonAltDegreesMeters.Latitude = glm::degrees(FMath::Atan((Ecef.Z + ePrimeSquared * zNot) / p));
	OutLatLonAltDegreesMeters.Longitude = glm::degrees(FMath::Atan2(Ecef.Y, Ecef.X));

	return true;
}

void UDIS_BPFL::CalculateEcefXYZFromLatLonAltitude(FGeographicCoordinates LatLonAltDegreesMeters, FVector& OutEcef)
{
	double earthEquitorialRadiusMeters = 6378137;
	double earthPolarRadiusMeters = 6356752.3142;

	double earthEquitorialRadiusMetersSquared = FMath::Square(earthEquitorialRadiusMeters);
	double earthPolarRadiusMetersSquared = FMath::Square(earthPolarRadiusMeters);

	double eSquared = 1 - earthPolarRadiusMetersSquared / earthEquitorialRadiusMetersSquared;
	double f = 1 - earthPolarRadiusMeters / earthEquitorialRadiusMeters;

	double nLat = earthEquitorialRadiusMeters / FMath::Sqrt(1 - eSquared * FMath::Square(FMath::Sin(glm::radians(LatLonAltDegreesMeters.Latitude))));

	double latRadians = glm::radians(LatLonAltDegreesMeters.Latitude);
	double lonRadians = glm::radians(LatLonAltDegreesMeters.Longitude);

	OutEcef.X = (nLat + LatLonAltDegreesMeters.Altitude) * FMath::Cos(latRadians) * FMath::Cos(lonRadians);
	OutEcef.Y = (nLat + LatLonAltDegreesMeters.Altitude) * FMath::Cos(latRadians) * FMath::Sin(lonRadians);
	OutEcef.Z = (FMath::Square(1 - f) * nLat + LatLonAltDegreesMeters.Altitude) * FMath::Sin(latRadians);
}

FMatrix UDIS_BPFL::CreateNCrossXMatrix(const FVector NVector)
{
	const auto NMatrix = FMatrix(FPlane(0, -NVector.Z, NVector.Y, 0),
		FPlane(NVector.Z, 0, -NVector.X, 0),
		FPlane(-NVector.Y, NVector.X, 0, 0),
		FPlane(0, 0, 0, 0)
	);

	return NMatrix;
}

glm::dmat3x3 UDIS_BPFL::CreateNCrossXMatrix(const glm::dvec3 NVector)
{
	return glm::dmat3x3(0, NVector.z, -NVector.y, -NVector.z, 0, NVector.x, NVector.y, -NVector.x, 0);
}

void UDIS_BPFL::CreateRotationMatrix(const FVector AxisVector, const float ThetaRadians, FMatrix& OutRotationMatrix)
{
	const float CosTheta = FMath::Cos(ThetaRadians);
	const float SinTheta = FMath::Sin(ThetaRadians);

	OutRotationMatrix = FMatrix(FPlane(FVector(0), 0), FPlane(FVector(0), 0), FVector4(FVector(0), 0), FVector4(FVector(0), 0));

	const auto N = FMatrix(FPlane(AxisVector, 0), FPlane(FVector(0), 0), FVector4(FVector(0), 0), FVector4(FVector(0), 0));
	const FMatrix NTransposeN = N.GetTransposed() * N;

	const FMatrix NCrossX = CreateNCrossXMatrix(AxisVector);

	auto ScaledTranspose = FMatrix(NTransposeN);
	ScaledTranspose *= (1 - CosTheta);
	auto Identity = FMatrix::Identity;
	Identity *= CosTheta;
	// Zero out the 4th row 4th column entry to represent the 3x3 matrix as a 4x4
	Identity.M[3][3] = 0;
	auto ScaledNCrossX = FMatrix(NCrossX);
	ScaledNCrossX *= SinTheta;

	OutRotationMatrix += ScaledTranspose + Identity + ScaledNCrossX;
}

void UDIS_BPFL::CreateRotationMatrix(const glm::dvec3 AxisVector, const double ThetaRadians, glm::dmat3x3& OutRotationMatrix)
{
	const double CosTheta = glm::cos(ThetaRadians);
	const double SinTheta = glm::sin(ThetaRadians);

	const auto NTransposeN = glm::outerProduct(AxisVector, AxisVector);
	const auto NCrossN = CreateNCrossXMatrix(AxisVector);

	OutRotationMatrix = ((1 - CosTheta) * NTransposeN) + (CosTheta * glm::identity<glm::dmat3x3>()) + (SinTheta * NCrossN);
}

void UDIS_BPFL::ApplyHeadingPitchToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the X and Y vectors around the Z vector by the Heading
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.NorthVector, HeadingDegrees, NorthEastDownVectors.DownVector, OutX);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, HeadingDegrees, NorthEastDownVectors.DownVector, OutY);

	// Rotate the X and Z vectors around the Y vector by the Pitch
	RotateVectorAroundAxisByDegrees(OutX, PitchDegrees, OutY, OutX);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, PitchDegrees, OutY, OutZ);
}

void UDIS_BPFL::ApplyRollToNorthEastDownVector(const float RollDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the Y and Z vectors around the X vector by the Roll
	OutX = NorthEastDownVectors.NorthVector;
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, RollDegrees, NorthEastDownVectors.NorthVector, OutY);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, RollDegrees, NorthEastDownVectors.NorthVector, OutZ);
}

void UDIS_BPFL::RotateVectorAroundAxisByRadians(const glm::dvec3 VectorToRotate, const double ThetaRadians, const glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	auto RotationMatrix = glm::dmat3x3();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	OutRotatedVector = RotationMatrix * VectorToRotate;
}

void UDIS_BPFL::RotateVectorAroundAxisByDegrees(const glm::dvec3 VectorToRotate, const float ThetaDegrees, const glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, glm::radians(ThetaDegrees), AxisVector, OutRotatedVector);
}

void UDIS_BPFL::RotateVectorAroundAxisByRadians(const FVector VectorToRotate, const float ThetaRadians, const FVector AxisVector, FVector& OutRotatedVector)
{
	const auto ZeroPlane = FPlane(FVector(0), 0);
	const auto VectorMatrix = FMatrix(FPlane(VectorToRotate, 0), ZeroPlane, ZeroPlane, ZeroPlane);
	auto RotationMatrix = FMatrix();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	const FMatrix ResMatrix = VectorMatrix * RotationMatrix.GetTransposed();
	OutRotatedVector = ResMatrix.GetScaledAxis(EAxis::X);
}

void UDIS_BPFL::RotateVectorAroundAxisByDegrees(const FVector VectorToRotate, const float ThetaDegrees, const FVector AxisVector, FVector& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, FMath::DegreesToRadians(ThetaDegrees), AxisVector, OutRotatedVector);
}

void UDIS_BPFL::ApplyHeadingPitchRollToNorthEastDownVector(const FHeadingPitchRoll HeadingPitchRollDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	ApplyHeadingPitchToNorthEastDownVector(HeadingPitchRollDegrees.Heading, HeadingPitchRollDegrees.Pitch, NorthEastDownVectors, OutX, OutY, OutZ);

	ApplyRollToNorthEastDownVector(HeadingPitchRollDegrees.Roll, FNorthEastDown(OutX, OutY, OutZ), OutX, OutY, OutZ);
}

void UDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(FGeographicCoordinates LatLonAltDegreesMeters, FNorthEastDown& NorthEastDownVectors)
{
	NorthEastDownVectors.NorthVector = FVector::ZAxisVector;
	NorthEastDownVectors.EastVector = FVector::YAxisVector;
	NorthEastDownVectors.DownVector = -FVector::XAxisVector;

	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, LatLonAltDegreesMeters.Longitude, NorthEastDownVectors.NorthVector, NorthEastDownVectors.EastVector);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, LatLonAltDegreesMeters.Longitude, NorthEastDownVectors.NorthVector, NorthEastDownVectors.DownVector);

	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.NorthVector, LatLonAltDegreesMeters.Latitude, -NorthEastDownVectors.EastVector, NorthEastDownVectors.NorthVector);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, LatLonAltDegreesMeters.Latitude, -NorthEastDownVectors.EastVector, NorthEastDownVectors.DownVector);
}

void UDIS_BPFL::CalculateLatLonFromNorthEastDownVectors(const FNorthEastDown NorthEastDownVectors, FGeographicCoordinates& LatLonAltDegreesMeters)
{
	LatLonAltDegreesMeters.Longitude = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::YAxisVector, NorthEastDownVectors.EastVector) / NorthEastDownVectors.EastVector.Size()));
	LatLonAltDegreesMeters.Latitude = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::ZAxisVector, NorthEastDownVectors.NorthVector) / NorthEastDownVectors.NorthVector.Size()));
}

void UDIS_BPFL::GetNEDVectorRotationOffset(const FNorthEastDown StartNEDVectors, const FNorthEastDown DestinationNEDVectors, double& RollOffset, double& PitchOffset, double& YawOffset)
{
	//Make needed matrices entries
	double offset_M11 = FVector::DotProduct(StartNEDVectors.EastVector, DestinationNEDVectors.EastVector);
	double offset_M21 = FVector::DotProduct(-StartNEDVectors.NorthVector, DestinationNEDVectors.EastVector);
	double offset_M31 = FVector::DotProduct(-StartNEDVectors.DownVector, DestinationNEDVectors.EastVector);
	double offset_M32 = FVector::DotProduct(-StartNEDVectors.DownVector, DestinationNEDVectors.NorthVector);
	double offset_M33 = FVector::DotProduct(-StartNEDVectors.DownVector, -DestinationNEDVectors.DownVector);

	//Use calculated matrices entries to find offsets of both origin and entity
	RollOffset = FMath::RadiansToDegrees(FMath::Atan2(offset_M32, offset_M33));
	PitchOffset = FMath::RadiansToDegrees(FMath::Atan2(offset_M31, FMath::Sqrt(FMath::Square(offset_M32) + FMath::Square(offset_M33))));
	YawOffset = FMath::RadiansToDegrees(FMath::Atan2(offset_M21, offset_M11));
}

void UDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, FGeographicCoordinates LatLonAltDegreesMeters, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	FNorthEastDown NorthEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatLonAltDegreesMeters, NorthEastDownVectors);

	FVector X, Y, Z, X2, Y2, Z2;
	ApplyHeadingPitchRollToNorthEastDownVector(HeadingPitchRollDegrees, NorthEastDownVectors, X, Y, Z);

	const auto X0 = FVector(1, 0, 0);
	const auto Y0 = FVector(0, 1, 0);
	const auto Z0 = FVector(0, 0, 1);

	PsiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(X, Y0), FVector::DotProduct(X, X0)));
	PsiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(FMath::Atan2(-FVector::DotProduct(X, Z0), FMath::Sqrt(FMath::Square(FVector::DotProduct(X, X0)) + FMath::Square(FVector::DotProduct(X, Y0)))));

	NorthEastDownVectors.NorthVector = X0;
	NorthEastDownVectors.EastVector = Y0;
	NorthEastDownVectors.DownVector = Z0;

	ApplyHeadingPitchToNorthEastDownVector(PsiThetaPhiDegrees.Psi, PsiThetaPhiDegrees.Theta, NorthEastDownVectors, X2, Y2, Z2);

	PsiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(Y, Z2), FVector::DotProduct(Y, Y2)));
}

void UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, FGeographicCoordinates LatLonAltDegreesMeters, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FHeadingPitchRoll HeadingPitchRollDegrees;
	HeadingPitchRollDegrees.Heading = FMath::RadiansToDegrees(HeadingPitchRollRadians.Heading);
	HeadingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(HeadingPitchRollRadians.Pitch);
	HeadingPitchRollDegrees.Roll = FMath::RadiansToDegrees(HeadingPitchRollRadians.Roll);

	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(HeadingPitchRollDegrees, LatLonAltDegreesMeters, PsiThetaPhiRadians);
}

void UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, FGeographicCoordinates LatLonAltDegreesMeters, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FPsiThetaPhi PsiThetaPhiDegrees;
	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(HeadingPitchRollDegrees, LatLonAltDegreesMeters, PsiThetaPhiDegrees);

	PsiThetaPhiRadians.Psi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Psi);
	PsiThetaPhiRadians.Theta = FMath::DegreesToRadians(PsiThetaPhiDegrees.Theta);
	PsiThetaPhiRadians.Phi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Phi);
}

void UDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, FGeographicCoordinates LatLonAltDegreesMeters, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	headingPitchRollDegrees.Heading = FMath::RadiansToDegrees(HeadingPitchRollRadians.Heading);
	headingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(HeadingPitchRollRadians.Pitch);
	headingPitchRollDegrees.Roll = FMath::RadiansToDegrees(HeadingPitchRollRadians.Roll);

	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, LatLonAltDegreesMeters, PsiThetaPhiDegrees);
}

void UDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, FGeographicCoordinates LatLonAltDegreesMeters, FHeadingPitchRoll& HeadingPitchRollDegrees)
{
	FNorthEastDown NorthEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatLonAltDegreesMeters, NorthEastDownVectors);

	const auto X0 = FVector(1, 0, 0);
	const auto Y0 = FVector(0, 1, 0);
	const auto Z0 = FVector(0, 0, 1);

	const FNorthEastDown StartingVectorsForRotation = FNorthEastDown(X0, Y0, Z0);

	FVector X3, Y3, Z3, X2, Y2, Z2;

	const FHeadingPitchRoll HeadingPitchRoll = FHeadingPitchRoll(PsiThetaPhiDegrees.Psi, PsiThetaPhiDegrees.Theta, PsiThetaPhiDegrees.Phi);

	ApplyHeadingPitchRollToNorthEastDownVector(HeadingPitchRoll, StartingVectorsForRotation, X3, Y3, Z3);

	HeadingPitchRollDegrees.Heading = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(X3, NorthEastDownVectors.EastVector), FVector::DotProduct(X3, NorthEastDownVectors.NorthVector)));
	HeadingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(FMath::Atan2(-FVector::DotProduct(X3, NorthEastDownVectors.DownVector), FMath::Sqrt(FMath::Square(FVector::DotProduct(X3, NorthEastDownVectors.EastVector)) + FMath::Square(FVector::DotProduct(X3, NorthEastDownVectors.NorthVector)))));

	ApplyHeadingPitchToNorthEastDownVector(HeadingPitchRollDegrees.Heading, HeadingPitchRollDegrees.Pitch, NorthEastDownVectors, X2, Y2, Z2);
	HeadingPitchRollDegrees.Roll = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(Y3, Z2), FVector::DotProduct(Y3, Y2)));
}

void UDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, FGeographicCoordinates LatLonAltDegreesMeters, FHeadingPitchRoll& HeadingPitchRollRadians)
{
	FPsiThetaPhi psiThetaPhiDegrees;
	psiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Psi);
	psiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(PsiThetaPhiRadians.Theta);
	psiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Phi);

	CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, LatLonAltDegreesMeters, HeadingPitchRollRadians);
}

void UDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, FGeographicCoordinates LatLonAltDegreesMeters, FHeadingPitchRoll& HeadingPitchRollDegrees)
{
	FPsiThetaPhi psiThetaPhiDegrees;
	psiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Psi);
	psiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(PsiThetaPhiRadians.Theta);
	psiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Phi);

	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, LatLonAltDegreesMeters, HeadingPitchRollDegrees);
}

void UDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, FGeographicCoordinates LatLonAltDegreesMeters, FHeadingPitchRoll& HeadingPitchRollRadians)
{
	FHeadingPitchRoll HeadingPitchRollDegrees;

	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(PsiThetaPhiDegrees, LatLonAltDegreesMeters, HeadingPitchRollDegrees);
	HeadingPitchRollRadians.Heading = FMath::DegreesToRadians(HeadingPitchRollDegrees.Heading);
	HeadingPitchRollRadians.Pitch = FMath::DegreesToRadians(HeadingPitchRollDegrees.Pitch);
	HeadingPitchRollRadians.Roll = FMath::DegreesToRadians(HeadingPitchRollDegrees.Roll);
}

void UDIS_BPFL::GetEcefXYZFromUnrealLocation(const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FVector& ECEF)
{
	if (!IsValid(GeoReferencingSystem))
	{
		ECEF = FVector();
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get EcefXYZ from. Returning ECEF XYZ of (0, 0, 0)."));
		return;
	}

	GeoReferencingSystem->EngineToECEF(UnrealLocation, ECEF);
}

void UDIS_BPFL::GetLatLonAltitudeFromUnrealLocation(const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FGeographicCoordinates& LatLonAltDegreesMeters)
{
	if (!IsValid(GeoReferencingSystem))
	{
		LatLonAltDegreesMeters = FGeographicCoordinates();
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get lat, lon, altitude from. Returning lat, lon, altitude of (0, 0, 0)."));
		return;
	}

	FVector ecefLoc;
	GeoReferencingSystem->EngineToECEF(UnrealLocation, ecefLoc);

	CalculateLatLonAltitudeFromEcefXYZ(ecefLoc, LatLonAltDegreesMeters);
}

void UDIS_BPFL::GetUnrealRotationFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, FGeographicCoordinates LatLonAltDegreesMeters, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal rotation from. Returning Unreal rotation of (0, 0, 0)."));
		return;
	}

	FHeadingPitchRoll headingPitchRollRadians;
	headingPitchRollRadians.Heading = FMath::DegreesToRadians(HeadingPitchRollDegrees.Heading);
	headingPitchRollRadians.Pitch = FMath::DegreesToRadians(HeadingPitchRollDegrees.Pitch);
	headingPitchRollRadians.Roll = FMath::DegreesToRadians(HeadingPitchRollDegrees.Roll);

	GetUnrealRotationFromHeadingPitchRollRadiansAtLatLon(headingPitchRollRadians, LatLonAltDegreesMeters, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealRotationFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, FGeographicCoordinates LatLonAltDegreesMeters, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal rotation from. Returning Unreal rotation of (0, 0, 0)."));
		return;
	}

	FPsiThetaPhi psiThetaPhiRadians;
	CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(HeadingPitchRollRadians, LatLonAltDegreesMeters, psiThetaPhiRadians);

	GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, LatLonAltDegreesMeters, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealRotationFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, FGeographicCoordinates LatLonAltDegreesMeters, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal rotation from. Returning Unreal rotation of (0, 0, 0)."));
		return;
	}

	FPsiThetaPhi psiThetaPhiRadians;
	psiThetaPhiRadians.Psi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Psi);
	psiThetaPhiRadians.Theta = FMath::DegreesToRadians(PsiThetaPhiDegrees.Theta);
	psiThetaPhiRadians.Phi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Phi);

	GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, LatLonAltDegreesMeters, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, FGeographicCoordinates LatLonAltDegreesMeters, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal rotation from. Returning Unreal rotation of (0, 0, 0)."));
		return;
	}

	FNorthEastDown NorthEastDownVectors;
	FVector entityEcef;
	CalculateEcefXYZFromLatLonAltitude(LatLonAltDegreesMeters, entityEcef);
	GeoReferencingSystem->GetENUVectorsAtECEFLocation(entityEcef, NorthEastDownVectors.EastVector, NorthEastDownVectors.NorthVector, NorthEastDownVectors.DownVector);
	NorthEastDownVectors.DownVector *= -1;

	//Get NED of the world origin
	FNorthEastDown originNorthEastDown;
	GetNorthEastDownVectorsFromUnrealLocation(FVector(0, 0, 0), GeoReferencingSystem, originNorthEastDown);

	double XAxisRotationAngle;
	double YAxisRotationAngle;
	double ZAxisRotationAngle;
	//Get rotation difference going from world origin NED to entity location NED
	GetNEDVectorRotationOffset(originNorthEastDown, NorthEastDownVectors, XAxisRotationAngle, YAxisRotationAngle, ZAxisRotationAngle);

	//Get the HPR that the entity would have
	FHeadingPitchRoll HeadingPitchRollDegrees;
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(PsiThetaPhiRadians, LatLonAltDegreesMeters, HeadingPitchRollDegrees);
	//Heading of 0 is East, but heading of 0 in Unreal is North. Subtract 90 to make up for the offset
	HeadingPitchRollDegrees.Heading -= 90;

	FVector unrealEntityLocation;
	GetUnrealLocationFromEcefXYZ(entityEcef, GeoReferencingSystem, unrealEntityLocation);

	FTransform entityTransform = FTransform(FRotator(YAxisRotationAngle, ZAxisRotationAngle, XAxisRotationAngle), unrealEntityLocation, FVector(1, 1, 1));
	FTransform originTransformWithHPR = FTransform(FRotator(HeadingPitchRollDegrees.Pitch, HeadingPitchRollDegrees.Heading, HeadingPitchRollDegrees.Roll), FVector(0, 0, 0), FVector(1, 1, 1));

	FTransform entityTransformWithHPR = originTransformWithHPR * entityTransform;

	UnrealRotation.Roll = entityTransformWithHPR.GetRotation().Rotator().Roll;
	UnrealRotation.Pitch = entityTransformWithHPR.GetRotation().Rotator().Pitch;
	UnrealRotation.Yaw = entityTransformWithHPR.GetRotation().Rotator().Yaw;
}

void UDIS_BPFL::GetUnrealLocationFromLatLonAltitude(const FVector LatLonAltDegreesMeters, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FVector ecefXYZFloat;
	CalculateEcefXYZFromLatLonAltitude(LatLonAltDegreesMeters, ecefXYZFloat);

	GetUnrealLocationFromEcefXYZ(ecefXYZFloat, GeoReferencingSystem, UnrealLocation);
}

void UDIS_BPFL::GetUnrealLocationFromEcefXYZ(const FVector EcefXYZ, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	GeoReferencingSystem->ECEFToEngine(EcefXYZ, UnrealLocation);
}

void UDIS_BPFL::GetUnrealRotationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal rotation from. Returning Unreal rotation of (0, 0, 0)."));
		return;
	}

	FPsiThetaPhi PsiThetaPhiRadians = FPsiThetaPhi(EntityStatePdu.EntityOrientation);

	FGeographicCoordinates LatLonAltDegreesMeters;
	CalculateLatLonAltitudeFromEcefXYZ(EntityStatePdu.EcefLocation, LatLonAltDegreesMeters);

	GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(PsiThetaPhiRadians, LatLonAltDegreesMeters, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealLocationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	GeoReferencingSystem->ECEFToEngine(EntityStatePdu.EcefLocation, UnrealLocation);
}

void UDIS_BPFL::GetUnrealLocationAndRotationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0,0,0);
		UnrealRotation = FRotator(0,0,0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location and rotation from. Returning location and rotation of (0, 0, 0)."));
		return;
	}

	GetUnrealLocationFromEntityStatePdu(EntityStatePdu, GeoReferencingSystem, UnrealLocation);
	GetUnrealRotationFromEntityStatePdu(EntityStatePdu, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetNorthEastDownVectorsFromUnrealLocation(const FVector UnrealLocation,	AGeoReferencingSystem* GeoReferencingSystem, FNorthEastDown& NorthEastDownVectors)
{
	if (!IsValid(GeoReferencingSystem))
	{
		NorthEastDownVectors = FNorthEastDown();
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get north, east, down vectors from. Returning north, east, down of (0, 0, 0)."));
		return;
	}

	GeoReferencingSystem->GetENUVectorsAtEngineLocation(UnrealLocation, NorthEastDownVectors.EastVector, NorthEastDownVectors.NorthVector, NorthEastDownVectors.DownVector);
	NorthEastDownVectors.DownVector *= -1;
}

void UDIS_BPFL::GetEastNorthUpVectorsFromUnrealLocation(const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FEastNorthUp& EastNorthUpVectors)
{
	FNorthEastDown NorthEastDownVectors;
	GetNorthEastDownVectorsFromUnrealLocation(UnrealLocation, GeoReferencingSystem, NorthEastDownVectors);
	GetEastNorthUpVectorsFromNorthEastDownVectors(NorthEastDownVectors, EastNorthUpVectors);
}

void UDIS_BPFL::GetHeadingPitchRollFromUnrealRotation(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FHeadingPitchRoll& HeadingPitchRollDegrees)
{
	if (!IsValid(GeoReferencingSystem))
	{
		HeadingPitchRollDegrees = FHeadingPitchRoll();
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get heading, pitch, roll rotation from. Returning heading, pitch, roll of (0, 0, 0)."));
		return;
	}

	FNorthEastDown NorthEastDownVectors;
	GetNorthEastDownVectorsFromUnrealLocation(UnrealLocation, GeoReferencingSystem, NorthEastDownVectors);

	//Get NED of the world origin
	FNorthEastDown originNorthEastDown;
	GetNorthEastDownVectorsFromUnrealLocation(FVector(0, 0, 0), GeoReferencingSystem, originNorthEastDown);

	double XAxisRotationAngle;
	double YAxisRotationAngle;
	double ZAxisRotationAngle;
	//Get rotation difference going from entity location NED to world origin NED
	GetNEDVectorRotationOffset(NorthEastDownVectors, originNorthEastDown, XAxisRotationAngle, YAxisRotationAngle, ZAxisRotationAngle);

	FTransform entityTransform = FTransform(FRotator(YAxisRotationAngle, ZAxisRotationAngle, XAxisRotationAngle), UnrealLocation, FVector(1, 1, 1));
	FTransform originTransformWithHPR = FTransform(FRotator(UnrealRotation.Pitch, UnrealRotation.Yaw, UnrealRotation.Roll), FVector(0, 0, 0), FVector(1, 1, 1));

	FTransform entityTransformWithHPR = originTransformWithHPR * entityTransform;

	HeadingPitchRollDegrees.Roll = entityTransformWithHPR.GetRotation().Rotator().Roll;
	HeadingPitchRollDegrees.Pitch = entityTransformWithHPR.GetRotation().Rotator().Pitch;
	//Heading of 0 is East, but heading of 0 in Unreal is North. Add 90 to make up for the offset
	HeadingPitchRollDegrees.Heading = entityTransformWithHPR.GetRotation().Rotator().Yaw + 90;
}

void UDIS_BPFL::GetPsiThetaPhiDegreesFromUnrealRotation(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	FGeographicCoordinates latLonAltDegreesMeters;
	GetHeadingPitchRollFromUnrealRotation(UnrealRotation, UnrealLocation, GeoReferencingSystem, headingPitchRollDegrees);
	GetLatLonAltitudeFromUnrealLocation(UnrealLocation, GeoReferencingSystem, latLonAltDegreesMeters);

	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonAltDegreesMeters, PsiThetaPhiDegrees);
}

void UDIS_BPFL::GetPsiThetaPhiRadiansFromUnrealRotation(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	FGeographicCoordinates latLonAltDegreesMeters;
	GetHeadingPitchRollFromUnrealRotation(UnrealRotation, UnrealLocation, GeoReferencingSystem, headingPitchRollDegrees);
	GetLatLonAltitudeFromUnrealLocation(UnrealLocation, GeoReferencingSystem, latLonAltDegreesMeters);

	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonAltDegreesMeters, PsiThetaPhiRadians);
}

void UDIS_BPFL::GetEcefXYZAndPsiThetaPhiDegreesFromUnreal(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FVector& EcefXYZ, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	GetEcefXYZFromUnrealLocation(UnrealLocation, GeoReferencingSystem, EcefXYZ);
	GetPsiThetaPhiDegreesFromUnrealRotation(UnrealRotation, UnrealLocation, GeoReferencingSystem, PsiThetaPhiDegrees);
}

void UDIS_BPFL::GetEcefXYZAndPsiThetaPhiRadiansFromUnreal(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FVector& EcefXYZ, FPsiThetaPhi& PsiThetaPhiRadians)
{
	GetEcefXYZFromUnrealLocation(UnrealLocation, GeoReferencingSystem, EcefXYZ);
	GetPsiThetaPhiRadiansFromUnrealRotation(UnrealRotation, UnrealLocation, GeoReferencingSystem, PsiThetaPhiRadians);
}

void UDIS_BPFL::GetEastNorthUpVectorsFromNorthEastDownVectors(const FNorthEastDown NorthEastDownVectors, FEastNorthUp& EastNorthUpVectors)
{
	const FMatrix NorthEastDownMatrix(NorthEastDownVectors.NorthVector, NorthEastDownVectors.EastVector, NorthEastDownVectors.DownVector, FVector(0));
	const FMatrix EastNorthUpMatrix = ConvertNedAndEnu(NorthEastDownMatrix);
	FVector NorthVector, EastVector, DownVector;
	EastNorthUpMatrix.GetUnitAxes(NorthVector, EastVector, DownVector);
	EastNorthUpVectors = FEastNorthUp(NorthVector, EastVector, DownVector);
}

void UDIS_BPFL::GetNorthEastDownVectorsFromEastNorthUpVectors(const FEastNorthUp EastNorthUpVectors, FNorthEastDown& NorthEastDownVectors)
{
	const FMatrix EastNorthUpMatrix(EastNorthUpVectors.EastVector, EastNorthUpVectors.NorthVector, EastNorthUpVectors.UpVector, FVector(0));
	const FMatrix NorthEastDownMatrix = ConvertNedAndEnu(EastNorthUpMatrix);
	FVector NorthVector, EastVector, DownVector;
	NorthEastDownMatrix.GetUnitAxes(NorthVector, EastVector, DownVector);
	NorthEastDownVectors = FNorthEastDown(NorthVector, EastVector, DownVector);
}

glm::dmat3 UDIS_BPFL::ConvertNedAndEnu(const glm::dmat3 StartingVectors)
{
	const glm::dmat3 ConversionMatrix = glm::dmat3(0, 1, 0, 1, 0, 0, 0, 0, -1);
	return StartingVectors * ConversionMatrix;
}

FMatrix UDIS_BPFL::ConvertNedAndEnu(const FMatrix StartingVectors)
{
	const FMatrix ConversionMatrix = FMatrix(FVector(0, 1, 0), FVector(1, 0, 0), FVector(0, 0, -1), FVector(0));
	return StartingVectors * ConversionMatrix;
}
