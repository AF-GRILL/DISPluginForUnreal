// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.


#include "DIS_BPFL.h"
#include "DISGameManager.h"

DEFINE_LOG_CATEGORY(LogDIS_BPFL);

void UDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(const FEarthCenteredEarthFixedDouble Ecef, FLatLonHeightDouble& OutLatLonHeightDegreesMeters)
{
	constexpr double earthEquitorialRadiusMeters = 6378137;
	constexpr double earthPolarRadiusMeters = 6356752.3142;

	const double earthEquitorialRadiusMetersSquared = FMath::Square(earthEquitorialRadiusMeters);
	const double earthPolarRadiusMetersSquared = FMath::Square(earthPolarRadiusMeters);

	double eSquared = (earthEquitorialRadiusMetersSquared - earthPolarRadiusMetersSquared) / earthEquitorialRadiusMetersSquared;
	double ePrimeSquared = (earthEquitorialRadiusMetersSquared - earthPolarRadiusMetersSquared) / earthPolarRadiusMetersSquared;

	double p = FMath::Sqrt(FMath::Square(Ecef.X) + FMath::Square(Ecef.Y));
	double F = 54 * earthPolarRadiusMetersSquared * FMath::Square(Ecef.Z);
	double G = FMath::Square(p) + (1 - eSquared) * FMath::Square(Ecef.Z) - eSquared * (earthEquitorialRadiusMetersSquared - earthPolarRadiusMetersSquared);
	double c = (FMath::Square(eSquared) * F * FMath::Square(p)) / FMath::Pow(G, 3);

	double s = FMath::Pow(1 + c + FMath::Sqrt(FMath::Square(c) + 2 * c), 1. / 3.);
	double k = s + 1 + 1 / s;
	double P = F / (3 * FMath::Square(k) * FMath::Square(G));
	double Q = FMath::Sqrt(1 + 2 * FMath::Square(eSquared) * P);

	double rNot = (-P * eSquared * p) / (1 + Q) + FMath::Sqrt(1. / 2. * earthEquitorialRadiusMetersSquared * (1 + 1 / Q) - (P * (1 - eSquared) * FMath::Square(Ecef.Z)) / (Q * (1 + Q)) - 1. / 2. * P * FMath::Square(p));
	double U = FMath::Sqrt(FMath::Square(p - eSquared * rNot) + FMath::Square(Ecef.Z));
	double V = FMath::Sqrt(FMath::Square(p - eSquared * rNot) + (1 - eSquared) * FMath::Square(Ecef.Z));
	double zNot = (earthPolarRadiusMetersSquared * Ecef.Z) / (earthEquitorialRadiusMeters * V);

	OutLatLonHeightDegreesMeters.Height = U * (1 - earthPolarRadiusMetersSquared / (earthEquitorialRadiusMeters * V));
	OutLatLonHeightDegreesMeters.Latitude = glm::degrees(FMath::Atan((Ecef.Z + ePrimeSquared * zNot) / p));
	OutLatLonHeightDegreesMeters.Longitude = glm::degrees(FMath::Atan2(Ecef.Y, Ecef.X));
}

void UDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(const FEarthCenteredEarthFixedFloat ECEF, FLatLonHeightFloat& OutLatLonHeightDegreesMeters)
{
	FLatLonHeightDouble LatLonHeightDouble;

	FEarthCenteredEarthFixedDouble EcefDouble;
	EcefDouble.X = static_cast<double>(ECEF.X);
	EcefDouble.Y = static_cast<double>(ECEF.Y);
	EcefDouble.Z = static_cast<double>(ECEF.Z);

	CalculateLatLonHeightFromEcefXYZ(EcefDouble, LatLonHeightDouble);


	OutLatLonHeightDegreesMeters.Latitude = LatLonHeightDouble.Latitude;
	OutLatLonHeightDegreesMeters.Longitude = LatLonHeightDouble.Longitude;
	OutLatLonHeightDegreesMeters.Height = LatLonHeightDouble.Height;
}

void UDIS_BPFL::CalculateEcefXYZFromLatLonHeight(const FLatLonHeightDouble LatLonHeightDegreesMeters, FEarthCenteredEarthFixedDouble& OutEcef)
{
	double earthEquitorialRadiusMeters = 6378137;
	double earthPolarRadiusMeters = 6356752.3142;

	double earthEquitorialRadiusMetersSquared = FMath::Square(earthEquitorialRadiusMeters);
	double earthPolarRadiusMetersSquared = FMath::Square(earthPolarRadiusMeters);

	double eSquared = 1 - earthPolarRadiusMetersSquared / earthEquitorialRadiusMetersSquared;
	double f = 1 - earthPolarRadiusMeters / earthEquitorialRadiusMeters;

	double nLat = earthEquitorialRadiusMeters / FMath::Sqrt(1 - eSquared * FMath::Square(FMath::Sin(glm::radians(LatLonHeightDegreesMeters.Latitude))));

	double latRadians = glm::radians(LatLonHeightDegreesMeters.Latitude);
	double lonRadians = glm::radians(LatLonHeightDegreesMeters.Longitude);

	OutEcef.X = (nLat + LatLonHeightDegreesMeters.Height) * FMath::Cos(latRadians) * FMath::Cos(lonRadians);
	OutEcef.Y = (nLat + LatLonHeightDegreesMeters.Height) * FMath::Cos(latRadians) * FMath::Sin(lonRadians);
	OutEcef.Z = (FMath::Square(1 - f) * nLat + LatLonHeightDegreesMeters.Height) * FMath::Sin(latRadians);
}

void UDIS_BPFL::CalculateEcefXYZFromLatLonHeight(const FLatLonHeightFloat LatLonHeightDegreesMeters, FEarthCenteredEarthFixedFloat& OutECEF)
{
	FEarthCenteredEarthFixedDouble ecefDouble;

	FLatLonHeightDouble latLonHeightDouble;
	latLonHeightDouble.Latitude = static_cast<double>(LatLonHeightDegreesMeters.Latitude);
	latLonHeightDouble.Longitude = static_cast<double>(LatLonHeightDegreesMeters.Longitude);
	latLonHeightDouble.Height = static_cast<double>(LatLonHeightDegreesMeters.Height);

	CalculateEcefXYZFromLatLonHeight(latLonHeightDouble, ecefDouble);

	OutECEF.X = ecefDouble.X;
	OutECEF.Y = ecefDouble.Y;
	OutECEF.Z = ecefDouble.Z;
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

void UDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(const float LatitudeDegrees, const float LongitudeDegrees, FNorthEastDown& NorthEastDownVectors)
{
	NorthEastDownVectors.NorthVector = FVector::ZAxisVector;
	NorthEastDownVectors.EastVector = FVector::YAxisVector;
	NorthEastDownVectors.DownVector = -FVector::XAxisVector;

	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, LongitudeDegrees, NorthEastDownVectors.NorthVector, NorthEastDownVectors.EastVector);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, LongitudeDegrees, NorthEastDownVectors.NorthVector, NorthEastDownVectors.DownVector);

	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.NorthVector, LatitudeDegrees, -NorthEastDownVectors.EastVector, NorthEastDownVectors.NorthVector);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, LatitudeDegrees, -NorthEastDownVectors.EastVector, NorthEastDownVectors.DownVector);
}

void UDIS_BPFL::CalculateLatLonFromNorthEastDownVectors(const FNorthEastDown NorthEastDownVectors, float& LatitudeDegrees, float& LongitudeDegrees)
{
	LongitudeDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::YAxisVector, NorthEastDownVectors.EastVector) / NorthEastDownVectors.EastVector.Size()));
	LatitudeDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::ZAxisVector, NorthEastDownVectors.NorthVector) / NorthEastDownVectors.NorthVector.Size()));
}

void UDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	FNorthEastDown NorthEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, NorthEastDownVectors);

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

void UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FHeadingPitchRoll HeadingPitchRollDegrees;
	HeadingPitchRollDegrees.Heading = FMath::RadiansToDegrees(HeadingPitchRollRadians.Heading);
	HeadingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(HeadingPitchRollRadians.Pitch);
	HeadingPitchRollDegrees.Roll = FMath::RadiansToDegrees(HeadingPitchRollRadians.Roll);

	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(HeadingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, PsiThetaPhiRadians);
}

void UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FPsiThetaPhi PsiThetaPhiDegrees;
	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(HeadingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, PsiThetaPhiDegrees);

	PsiThetaPhiRadians.Psi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Psi);
	PsiThetaPhiRadians.Theta = FMath::DegreesToRadians(PsiThetaPhiDegrees.Theta);
	PsiThetaPhiRadians.Phi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Phi);
}

void UDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	headingPitchRollDegrees.Heading = FMath::RadiansToDegrees(HeadingPitchRollRadians.Heading);
	headingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(HeadingPitchRollRadians.Pitch);
	headingPitchRollDegrees.Roll = FMath::RadiansToDegrees(HeadingPitchRollRadians.Roll);

	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, PsiThetaPhiDegrees);
}

void UDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollDegrees)
{
	FNorthEastDown NorthEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, NorthEastDownVectors);

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

void UDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollRadians)
{
	FPsiThetaPhi psiThetaPhiDegrees;
	psiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Psi);
	psiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(PsiThetaPhiRadians.Theta);
	psiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Phi);

	CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingPitchRollRadians);
}

void UDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollDegrees)
{
	FPsiThetaPhi psiThetaPhiDegrees;
	psiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Psi);
	psiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(PsiThetaPhiRadians.Theta);
	psiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Phi);

	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingPitchRollDegrees);
}

void UDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollRadians)
{
	FHeadingPitchRoll HeadingPitchRollDegrees;

	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(PsiThetaPhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingPitchRollDegrees);
	HeadingPitchRollRadians.Heading = FMath::DegreesToRadians(HeadingPitchRollDegrees.Heading);
	HeadingPitchRollRadians.Pitch = FMath::DegreesToRadians(HeadingPitchRollDegrees.Pitch);
	HeadingPitchRollRadians.Roll = FMath::DegreesToRadians(HeadingPitchRollDegrees.Roll);
}

void UDIS_BPFL::GetEcefXYZFromUnrealLocation(const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FEarthCenteredEarthFixedFloat& ECEF)
{
	if (!IsValid(GeoReferencingSystem))
	{
		ECEF = FEarthCenteredEarthFixedFloat();
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get EcefXYZ from. Returning ECEF XYZ of (0, 0, 0)."));
		return;
	}

	FCartesianCoordinates cartCoords;
	GeoReferencingSystem->EngineToECEF(UnrealLocation, cartCoords);

	ECEF.X = cartCoords.X;
	ECEF.Y = cartCoords.Y;
	ECEF.Z = cartCoords.Z;
}

void UDIS_BPFL::GetLatLonHeightFromUnrealLocation(const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FLatLonHeightFloat& LatLonHeightDegreesMeters)
{
	if (!IsValid(GeoReferencingSystem))
	{
		LatLonHeightDegreesMeters = FLatLonHeightFloat();
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get lat, lon, height from. Returning lat, lon, height of (0, 0, 0)."));
		return;
	}

	FCartesianCoordinates cartCoords;
	GeoReferencingSystem->EngineToECEF(UnrealLocation, cartCoords);

	FEarthCenteredEarthFixedDouble ecefDouble = FEarthCenteredEarthFixedDouble(cartCoords.X, cartCoords.Y, cartCoords.Z);
	FLatLonHeightDouble llhDouble;
	CalculateLatLonHeightFromEcefXYZ(ecefDouble, llhDouble);

	LatLonHeightDegreesMeters.Latitude = llhDouble.Latitude;
	LatLonHeightDegreesMeters.Longitude = llhDouble.Longitude;
	LatLonHeightDegreesMeters.Height = llhDouble.Height;
}

void UDIS_BPFL::GetUnrealRotationFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FHeadingPitchRoll headingPitchRollRadians;
	headingPitchRollRadians.Heading = FMath::DegreesToRadians(HeadingPitchRollDegrees.Heading);
	headingPitchRollRadians.Pitch = FMath::DegreesToRadians(HeadingPitchRollDegrees.Pitch);
	headingPitchRollRadians.Roll = FMath::DegreesToRadians(HeadingPitchRollDegrees.Roll);

	GetUnrealRotationFromHeadingPitchRollRadiansAtLatLon(headingPitchRollRadians, LatitudeDegrees, LongitudeDegrees, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealRotationFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FPsiThetaPhi psiThetaPhiRadians;
	CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(HeadingPitchRollRadians, LatitudeDegrees, LongitudeDegrees, psiThetaPhiRadians);

	GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, LatitudeDegrees, LongitudeDegrees, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealRotationFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FPsiThetaPhi psiThetaPhiRadians;
	psiThetaPhiRadians.Psi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Psi);
	psiThetaPhiRadians.Theta = FMath::DegreesToRadians(PsiThetaPhiDegrees.Theta);
	psiThetaPhiRadians.Phi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Phi);

	GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, LatitudeDegrees, LongitudeDegrees, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, AGeoReferencingSystem* GeoReferencingSystem, FRotator& UnrealRotation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealRotation = FRotator(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FNorthEastDown NorthEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, NorthEastDownVectors);

	//Get NED of the world origin
	FNorthEastDown originNorthEastDown;
	GeoReferencingSystem->GetENUVectorsAtEngineLocation(FVector(0, 0, 0), originNorthEastDown.EastVector, originNorthEastDown.NorthVector, originNorthEastDown.DownVector);
	originNorthEastDown.DownVector *= -1;

	// Get the rotational difference between calculated NED and Unreal origin NED
	const auto XAxisRotationAngle = FVector::DotProduct(NorthEastDownVectors.EastVector, originNorthEastDown.EastVector);
	const auto YAxisRotationAngle = FVector::DotProduct(NorthEastDownVectors.DownVector, originNorthEastDown.DownVector);
	const auto ZAxisRotationAngle = FVector::DotProduct(NorthEastDownVectors.NorthVector, originNorthEastDown.NorthVector);

	FHeadingPitchRoll HeadingPitchRollDegrees;
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(PsiThetaPhiRadians, LatitudeDegrees, LongitudeDegrees, HeadingPitchRollDegrees);

	UnrealRotation.Roll = HeadingPitchRollDegrees.Roll + XAxisRotationAngle;
	UnrealRotation.Pitch = HeadingPitchRollDegrees.Pitch + YAxisRotationAngle;
	//Heading of 0 is East, but heading of 0 in Unreal is North. Subtract 90 to make up for the offset
	UnrealRotation.Yaw = HeadingPitchRollDegrees.Heading + ZAxisRotationAngle - 90;
}

void UDIS_BPFL::GetUnrealLocationFromLatLonHeight(const FLatLonHeightFloat LatLonHeight, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FEarthCenteredEarthFixedFloat ecefXYZFloat;
	CalculateEcefXYZFromLatLonHeight(LatLonHeight, ecefXYZFloat);

	GetUnrealLocationFromEcefXYZ(ecefXYZFloat, GeoReferencingSystem, UnrealLocation);
}

void UDIS_BPFL::GetUnrealLocationFromEcefXYZ(const FEarthCenteredEarthFixedFloat EcefXYZ, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FEarthCenteredEarthFixedDouble EcefXYZDouble;
	EcefXYZDouble.X = static_cast<double>(EcefXYZ.X);
	EcefXYZDouble.Y = static_cast<double>(EcefXYZ.Y);
	EcefXYZDouble.Z = static_cast<double>(EcefXYZ.Z);
	GetUnrealLocationFromEcefXYZ(EcefXYZDouble, GeoReferencingSystem, UnrealLocation);
}

void UDIS_BPFL::GetUnrealLocationFromEcefXYZ(const FEarthCenteredEarthFixedDouble EcefXYZ, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FCartesianCoordinates cartCoords = FCartesianCoordinates(EcefXYZ.X, EcefXYZ.Y, EcefXYZ.Z);

	GeoReferencingSystem->ECEFToEngine(cartCoords, UnrealLocation);
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

	FEarthCenteredEarthFixedDouble ecefDouble = FEarthCenteredEarthFixedDouble(EntityStatePdu.EntityLocationDouble[0], EntityStatePdu.EntityLocationDouble[1], EntityStatePdu.EntityLocationDouble[2]);

	FLatLonHeightDouble LatLonHeightDouble;
	CalculateLatLonHeightFromEcefXYZ(ecefDouble, LatLonHeightDouble);

	GetUnrealRotationFromPsiThetaPhiRadiansAtLatLon(PsiThetaPhiRadians, LatLonHeightDouble.Latitude, LatLonHeightDouble.Longitude, GeoReferencingSystem, UnrealRotation);
}

void UDIS_BPFL::GetUnrealLocationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation)
{
	if (!IsValid(GeoReferencingSystem))
	{
		UnrealLocation = FVector(0, 0, 0);
		UE_LOG(LogDIS_BPFL, Warning, TEXT("Invalid GeoReference was passed to get Unreal location from. Returning Unreal location of (0, 0, 0)."));
		return;
	}

	FEarthCenteredEarthFixedDouble ecefDouble = FEarthCenteredEarthFixedDouble(EntityStatePdu.EntityLocationDouble[0], EntityStatePdu.EntityLocationDouble[1], EntityStatePdu.EntityLocationDouble[2]);

	FCartesianCoordinates cartCoords = FCartesianCoordinates(ecefDouble.X, ecefDouble.Y, ecefDouble.Z);

	GeoReferencingSystem->ECEFToEngine(cartCoords, UnrealLocation);
}

void UDIS_BPFL::GetUnrealLocationAndOrientationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& UnrealLocation, FRotator& UnrealRotation)
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
	FNorthEastDown OriginNorthEastDown;
	GeoReferencingSystem->GetENUVectorsAtEngineLocation(FVector(0, 0, 0), OriginNorthEastDown.EastVector, OriginNorthEastDown.NorthVector, OriginNorthEastDown.DownVector);
	OriginNorthEastDown.DownVector *= -1;

	// Get the rotational difference between calculated NED and Unreal origin NED
	const auto XAxisRotationAngle = FMath::Acos(FVector::DotProduct(NorthEastDownVectors.EastVector, OriginNorthEastDown.EastVector));
	const auto YAxisRotationAngle = FMath::Acos(FVector::DotProduct(NorthEastDownVectors.DownVector, OriginNorthEastDown.DownVector));
	const auto ZAxisRotationAngle = FMath::Acos(FVector::DotProduct(NorthEastDownVectors.NorthVector, OriginNorthEastDown.NorthVector));

	HeadingPitchRollDegrees.Roll = UnrealRotation.Roll - XAxisRotationAngle;
	HeadingPitchRollDegrees.Pitch = UnrealRotation.Pitch - YAxisRotationAngle;
	//Heading of 0 is East, but heading of 0 in Unreal is North. Add 90 to make up for the offset
	HeadingPitchRollDegrees.Heading = UnrealRotation.Yaw - ZAxisRotationAngle + 90;
}

void UDIS_BPFL::GetPsiThetaPhiDegreesFromUnrealRotation(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	FLatLonHeightFloat latLonHeightDegrees;
	GetHeadingPitchRollFromUnrealRotation(UnrealRotation, UnrealLocation, GeoReferencingSystem, headingPitchRollDegrees);
	GetLatLonHeightFromUnrealLocation(UnrealLocation, GeoReferencingSystem, latLonHeightDegrees);

	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonHeightDegrees.Latitude, latLonHeightDegrees.Longitude, PsiThetaPhiDegrees);
}

void UDIS_BPFL::GetPsiThetaPhiRadiansFromUnrealRotation(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	FLatLonHeightFloat latLonHeightDegrees;
	GetHeadingPitchRollFromUnrealRotation(UnrealRotation, UnrealLocation, GeoReferencingSystem, headingPitchRollDegrees);
	GetLatLonHeightFromUnrealLocation(UnrealLocation, GeoReferencingSystem, latLonHeightDegrees);

	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, latLonHeightDegrees.Latitude, latLonHeightDegrees.Longitude, PsiThetaPhiRadians);
}

void UDIS_BPFL::GetEcefXYZAndPsiThetaPhiDegreesFromUnreal(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FEarthCenteredEarthFixedFloat& EcefXYZ, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	GetEcefXYZFromUnrealLocation(UnrealLocation, GeoReferencingSystem, EcefXYZ);
	GetPsiThetaPhiDegreesFromUnrealRotation(UnrealRotation, UnrealLocation, GeoReferencingSystem, PsiThetaPhiDegrees);
}

void UDIS_BPFL::GetEcefXYZAndPsiThetaPhiRadiansFromUnreal(const FRotator UnrealRotation, const FVector UnrealLocation, AGeoReferencingSystem* GeoReferencingSystem, FEarthCenteredEarthFixedFloat& EcefXYZ, FPsiThetaPhi& PsiThetaPhiRadians)
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