// Fill out your copyright notice in the Description page of Project Settings.


#include "UEOpenDIS_BPFL.h"
#include "UEOpenDISGameState.h"

void UUEOpenDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(const FEarthCenteredEarthFixedDouble Ecef,	FLatLonHeightDouble& OutLatLonHeightDegreesMeters)
{
	constexpr double EarthSemiMajorRadiusMeters = 6378137;
	constexpr double EarthSemiMinorRadiusMeters = 6356752.3142;

	const double Longitude = FMath::RadiansToDegrees(FMath::Atan2(Ecef.Y, Ecef.X));
	// Latitude accurate to ~5 decimal places
	const double Latitude = FMath::RadiansToDegrees(FMath::Atan((FMath::Square(EarthSemiMajorRadiusMeters) / FMath::Square(EarthSemiMinorRadiusMeters))*(Ecef.Z / FMath::Sqrt(FMath::Square(Ecef.X) + FMath::Square(Ecef.Y)))));

	const double EarthSemiMajorRadiusMetersSquare = FMath::Square(EarthSemiMajorRadiusMeters);
	const double EarthSemiMinorRadiusMetersSquare = FMath::Square(EarthSemiMinorRadiusMeters);
	const double DistFromXToY = FMath::Sqrt(FMath::Square(Ecef.X) + FMath::Square(Ecef.Y));
	const double CosLatitude = FMath::Cos(FMath::DegreesToRadians(Latitude));
	const double SinLatitude = FMath::Sin(FMath::DegreesToRadians(Latitude));
	const double Height = (DistFromXToY / CosLatitude) - (EarthSemiMajorRadiusMetersSquare / FMath::Sqrt(
		(EarthSemiMajorRadiusMetersSquare * FMath::Square(CosLatitude)) + (EarthSemiMinorRadiusMetersSquare *
			FMath::Square(SinLatitude))));

	OutLatLonHeightDegreesMeters.Latitude = Latitude;
	OutLatLonHeightDegreesMeters.Longitude = Longitude;
	OutLatLonHeightDegreesMeters.Height = Height;
}

void UUEOpenDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(const FEarthCenteredEarthFixedFloat ECEF, FLatLonHeightFloat& OutLatLonHeightDegreesMeters)
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

void UUEOpenDIS_BPFL::CalculateEcefXYZFromLatLonHeight(const FLatLonHeightDouble LatLonHeightDegreesMeters, FEarthCenteredEarthFixedDouble& OutEcef)
{
	constexpr double EarthSemiMajorRadiusMeters = 6378137;
	constexpr double EarthSemiMinorRadiusMeters = 6356752.3142;

	const double CosLatitude = glm::cos(glm::radians(LatLonHeightDegreesMeters.Latitude));
	const double SinLatitude = glm::sin(glm::radians(LatLonHeightDegreesMeters.Latitude));
	const double CosLongitude = FMath::Cos(FMath::DegreesToRadians(LatLonHeightDegreesMeters.Longitude));
	const double SinLongitude = FMath::Sin(FMath::DegreesToRadians(LatLonHeightDegreesMeters.Longitude));

	const double XYBaseConversion = (EarthSemiMajorRadiusMeters / (FMath::Sqrt(FMath::Square(CosLatitude) + ((FMath::Square(EarthSemiMinorRadiusMeters) / FMath::Square(EarthSemiMajorRadiusMeters)) * FMath::Square(SinLatitude))))) + LatLonHeightDegreesMeters.Height;
	const double ZBaseConversion = (EarthSemiMinorRadiusMeters / (((FMath::Sqrt(FMath::Square(CosLatitude) * (FMath::Square(EarthSemiMajorRadiusMeters) / FMath::Square(EarthSemiMinorRadiusMeters)) + FMath::Square(SinLatitude)))))) + LatLonHeightDegreesMeters.Height;

	DIS::Vector3Double Location;

	const double EcefX = XYBaseConversion * CosLatitude * CosLongitude;
	const double EcefY = XYBaseConversion * CosLatitude * SinLongitude;
	const double EcefZ = ZBaseConversion * SinLatitude;

	Location.setX(EcefX);
	Location.setY(EcefY);
	Location.setZ(EcefZ);

	OutEcef.X = EcefX;
	OutEcef.Y = EcefY;
	OutEcef.Z = EcefZ;
}

void UUEOpenDIS_BPFL::CalculateEcefXYZFromLatLonHeight(const FLatLonHeightFloat LatLonHeightDegreesMeters, FEarthCenteredEarthFixedFloat& OutECEF)
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


FMatrix UUEOpenDIS_BPFL::CreateNCrossXMatrix(const FVector NVector)
{
	const auto NMatrix = FMatrix(FPlane(0, -NVector.Z, NVector.Y, 0),
		FPlane(NVector.Z, 0, -NVector.X, 0),
		FPlane(-NVector.Y, NVector.X, 0, 0),
		FPlane(0, 0, 0, 0)
	);

	return NMatrix;
}

glm::dmat3x3 UUEOpenDIS_BPFL::CreateNCrossXMatrix(const glm::dvec3 NVector)
{
	return glm::dmat3x3(0, NVector.z, -NVector.y, -NVector.z, 0, NVector.x, NVector.y, -NVector.x, 0);
}

void UUEOpenDIS_BPFL::CreateRotationMatrix(const FVector AxisVector, const float ThetaRadians, FMatrix& OutRotationMatrix)
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

void UUEOpenDIS_BPFL::CreateRotationMatrix(const glm::dvec3 AxisVector, const double ThetaRadians, glm::dmat3x3& OutRotationMatrix)
{
	const double CosTheta = glm::cos(ThetaRadians);
	const double SinTheta = glm::sin(ThetaRadians);

	const auto N = AxisVector;
	const auto NMat = glm::dmat3(N, glm::dvec3(0), glm::dvec3(0));

	const auto NTransposeN = NMat * glm::transpose(NMat);
	const auto NCrossN = CreateNCrossXMatrix(N);

	OutRotationMatrix = ((1 - CosTheta) * NTransposeN) + (CosTheta * glm::identity<glm::dmat3x3>()) + (SinTheta * NCrossN);
}

void UUEOpenDIS_BPFL::ApplyHeadingPitchToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the X and Y vectors around the Z vector by the Heading
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.NorthVector, HeadingDegrees, NorthEastDownVectors.DownVector, OutX);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, HeadingDegrees, NorthEastDownVectors.DownVector, OutY);

	// Rotate the X and Z vectors around the Y vector by the Pitch
	RotateVectorAroundAxisByDegrees(OutX, PitchDegrees, OutY, OutX);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, PitchDegrees, OutY, OutZ);
}

void UUEOpenDIS_BPFL::ApplyRollToNorthEastDownVector(const float RollDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the Y and Z vectors around the X vector by the Roll
	OutX = NorthEastDownVectors.NorthVector;
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, RollDegrees, NorthEastDownVectors.NorthVector, OutY);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, RollDegrees, NorthEastDownVectors.NorthVector, OutZ);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByRadians(const glm::dvec3 VectorToRotate, const double ThetaRadians, const glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	auto RotationMatrix = glm::dmat3x3();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	OutRotatedVector = RotationMatrix * VectorToRotate;
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByDegrees(const glm::dvec3 VectorToRotate, const float ThetaDegrees, const glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, glm::radians(ThetaDegrees), AxisVector, OutRotatedVector);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByRadians(const FVector VectorToRotate, const float ThetaRadians, const FVector AxisVector, FVector& OutRotatedVector)
{
	const auto ZeroPlane = FPlane(FVector(0), 0);
	const auto VectorMatrix = FMatrix(FPlane(VectorToRotate, 0), ZeroPlane, ZeroPlane, ZeroPlane);
	auto RotationMatrix = FMatrix();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	const FMatrix ResMatrix = VectorMatrix * RotationMatrix.GetTransposed();
	OutRotatedVector = ResMatrix.GetScaledAxis(EAxis::X);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByDegrees(const FVector VectorToRotate, const float ThetaDegrees, const FVector AxisVector, FVector& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, FMath::DegreesToRadians(ThetaDegrees), AxisVector, OutRotatedVector);
}

void UUEOpenDIS_BPFL::ApplyHeadingPitchRollToNorthEastDownVector(const FHeadingPitchRoll HeadingPitchRollDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	ApplyHeadingPitchToNorthEastDownVector(HeadingPitchRollDegrees.Heading, HeadingPitchRollDegrees.Pitch, NorthEastDownVectors, OutX, OutY, OutZ);

	ApplyRollToNorthEastDownVector(HeadingPitchRollDegrees.Roll, FNorthEastDown(OutX, OutY, OutZ), OutX, OutY, OutZ);
}

void UUEOpenDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(const float LatitudeDegrees, const float LongitudeDegrees, FNorthEastDown& NorthEastDownVectors)
{
	NorthEastDownVectors.NorthVector = FVector::ZAxisVector;
	NorthEastDownVectors.EastVector = FVector::YAxisVector;
	NorthEastDownVectors.DownVector = -FVector::XAxisVector;

	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, LongitudeDegrees, NorthEastDownVectors.NorthVector, NorthEastDownVectors.EastVector);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, LongitudeDegrees, NorthEastDownVectors.NorthVector, NorthEastDownVectors.DownVector);

	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.NorthVector, LatitudeDegrees, -NorthEastDownVectors.EastVector, NorthEastDownVectors.NorthVector);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, LatitudeDegrees, -NorthEastDownVectors.EastVector, NorthEastDownVectors.DownVector);
}

void UUEOpenDIS_BPFL::CalculateLatLongFromNorthEastDownVectors(const FNorthEastDown NorthEastDownVectors, float& LatitudeDegrees, float& LongitudeDegrees)
{
	LongitudeDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::YAxisVector, NorthEastDownVectors.EastVector) / NorthEastDownVectors.EastVector.Size()));
	LatitudeDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::ZAxisVector, NorthEastDownVectors.NorthVector) / NorthEastDownVectors.NorthVector.Size()));
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiDegrees)
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

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FHeadingPitchRoll HeadingPitchRollDegrees;
	HeadingPitchRollDegrees.Heading = FMath::RadiansToDegrees(HeadingPitchRollRadians.Heading);
	HeadingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(HeadingPitchRollRadians.Pitch);
	HeadingPitchRollDegrees.Roll = FMath::RadiansToDegrees(HeadingPitchRollRadians.Roll);

	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(HeadingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, PsiThetaPhiRadians);
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FPsiThetaPhi PsiThetaPhiDegrees;
	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(HeadingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, PsiThetaPhiDegrees);

	PsiThetaPhiRadians.Psi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Psi);
	PsiThetaPhiRadians.Theta = FMath::DegreesToRadians(PsiThetaPhiDegrees.Theta);
	PsiThetaPhiRadians.Phi = FMath::DegreesToRadians(PsiThetaPhiDegrees.Phi);
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiDegrees)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	headingPitchRollDegrees.Heading = FMath::RadiansToDegrees(HeadingPitchRollRadians.Heading);
	headingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(HeadingPitchRollRadians.Pitch);
	headingPitchRollDegrees.Roll = FMath::RadiansToDegrees(HeadingPitchRollRadians.Roll);

	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, PsiThetaPhiDegrees);
}

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollDegrees)
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

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollRadians)
{
	FPsiThetaPhi psiThetaPhiDegrees;
	psiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Psi);
	psiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(PsiThetaPhiRadians.Theta);
	psiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Phi);

	CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingPitchRollRadians);
}

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(const FPsiThetaPhi PsiThetaPhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollDegrees)
{
	FPsiThetaPhi psiThetaPhiDegrees;
	psiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Psi);
	psiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(PsiThetaPhiRadians.Theta);
	psiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(PsiThetaPhiRadians.Phi);

	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingPitchRollDegrees);
}

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(const FPsiThetaPhi PsiThetaPhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FHeadingPitchRoll& HeadingPitchRollRadians)
{
	FHeadingPitchRoll HeadingPitchRollDegrees;

	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(PsiThetaPhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingPitchRollDegrees);
	HeadingPitchRollRadians.Heading = FMath::DegreesToRadians(HeadingPitchRollDegrees.Heading);
	HeadingPitchRollRadians.Pitch = FMath::DegreesToRadians(HeadingPitchRollDegrees.Pitch);
	HeadingPitchRollRadians.Roll = FMath::DegreesToRadians(HeadingPitchRollDegrees.Roll);
}

void UUEOpenDIS_BPFL::CalculateEcefXYZFromUnrealLocation(const FVector UELocation, AGeoReferencingSystem* GeoReferencingSystem, FEarthCenteredEarthFixedFloat& ECEF)
{
	FCartesianCoordinates cartCoords;
	GeoReferencingSystem->EngineToECEF(UELocation, cartCoords);

	ECEF.X = cartCoords.X;
	ECEF.Y = cartCoords.Y;
	ECEF.Z = cartCoords.Z;
}

void UUEOpenDIS_BPFL::CalculateLatLonHeightFromUnrealLocation(const FVector UELocation, AGeoReferencingSystem* GeoReferencingSystem, FLatLonHeightFloat& LatLonHeightDegreesMeters)
{
	FCartesianCoordinates cartCoords;
	GeoReferencingSystem->EngineToECEF(UELocation, cartCoords);

	FEarthCenteredEarthFixedDouble ecefDouble = FEarthCenteredEarthFixedDouble(cartCoords.X, cartCoords.Y, cartCoords.Z);
	FLatLonHeightDouble llhDouble;
	CalculateLatLonHeightFromEcefXYZ(ecefDouble, llhDouble);

	LatLonHeightDegreesMeters.Latitude = llhDouble.Latitude;
	LatLonHeightDegreesMeters.Longitude = llhDouble.Longitude;
	LatLonHeightDegreesMeters.Height = llhDouble.Height;
}

void UUEOpenDIS_BPFL::GetUnrealRotationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FRotator& EntityRotation)
{
	FEarthCenteredEarthFixedDouble ecefDouble = FEarthCenteredEarthFixedDouble(EntityStatePdu.EntityLocationDouble[0], EntityStatePdu.EntityLocationDouble[1], EntityStatePdu.EntityLocationDouble[2]);

	FLatLonHeightDouble LatLonHeightDouble;
	CalculateLatLonHeightFromEcefXYZ(ecefDouble, LatLonHeightDouble);

	FNorthEastDown NorthEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatLonHeightDouble.Latitude, LatLonHeightDouble.Longitude, NorthEastDownVectors);

	FCartesianCoordinates cartCoords = FCartesianCoordinates(GeoReferencingSystem->OriginProjectedCoordinatesEasting, GeoReferencingSystem->OriginProjectedCoordinatesNorthing, GeoReferencingSystem->OriginProjectedCoordinatesUp);
	FNorthEastDown originNorthEastDown;

	GeoReferencingSystem->GetENUVectorsAtProjectedLocation(cartCoords, originNorthEastDown.EastVector, originNorthEastDown.NorthVector, originNorthEastDown.DownVector);
	originNorthEastDown.DownVector *= -1;

	// Get the rotational difference between calculated NED and Unreal origin NED
	const auto XAxisRotationAngle = FVector::DotProduct(NorthEastDownVectors.EastVector, originNorthEastDown.EastVector);
	const auto YAxisRotationAngle = FVector::DotProduct(NorthEastDownVectors.DownVector, originNorthEastDown.DownVector);
	const auto ZAxisRotationAngle = FVector::DotProduct(NorthEastDownVectors.NorthVector, originNorthEastDown.NorthVector);

	FPsiThetaPhi PsiThetaPhiRadians = FPsiThetaPhi(EntityStatePdu.EntityOrientation.Yaw, EntityStatePdu.EntityOrientation.Pitch, EntityStatePdu.EntityOrientation.Roll);

	FHeadingPitchRoll HeadingPitchRollDegrees;
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(PsiThetaPhiRadians, LatLonHeightDouble.Latitude, LatLonHeightDouble.Longitude, HeadingPitchRollDegrees);

	EntityRotation.Roll = HeadingPitchRollDegrees.Roll + XAxisRotationAngle;
	EntityRotation.Pitch = HeadingPitchRollDegrees.Pitch + YAxisRotationAngle;
	EntityRotation.Yaw = HeadingPitchRollDegrees.Heading + ZAxisRotationAngle;
}

void UUEOpenDIS_BPFL::GetEntityUnrealLocationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& EntityLocation)
{
	FEarthCenteredEarthFixedDouble ecefDouble = FEarthCenteredEarthFixedDouble(EntityStatePdu.EntityLocationDouble[0], EntityStatePdu.EntityLocationDouble[1], EntityStatePdu.EntityLocationDouble[2]);

	FCartesianCoordinates cartCoords = FCartesianCoordinates(ecefDouble.X, ecefDouble.Y, ecefDouble.Z);

	GeoReferencingSystem->ECEFToEngine(cartCoords, EntityLocation);
}

void UUEOpenDIS_BPFL::GetEntityUnrealLocationAndOrientation(const FEntityStatePDU EntityStatePdu, AGeoReferencingSystem* GeoReferencingSystem, FVector& EntityLocation, FRotator& EntityRotation)
{
	GetEntityUnrealLocationFromEntityStatePdu(EntityStatePdu, GeoReferencingSystem, EntityLocation);
	GetUnrealRotationFromEntityStatePdu(EntityStatePdu, GeoReferencingSystem, EntityRotation);
}