// Fill out your copyright notice in the Description page of Project Settings.


#include "UEOpenDIS_BPFL.h"
#include "UEOpenDISGameState.h"

#include "Kismet/KismetMathLibrary.h"

double UUEOpenDIS_BPFL::GetHeadingFromEuler(double LatitudeRadians, double LongitudeRadians, float PsiRadians, float ThetaRadians)
{
	double sinLat = FMath::Sin(LatitudeRadians);
	double sinLon = FMath::Sin(LongitudeRadians);
	double cosLon = FMath::Cos(LongitudeRadians);
	double cosLat = FMath::Cos(LatitudeRadians);
	double sinSin = sinLat * sinLon;

	double cosTheta = FMath::Cos(ThetaRadians);
	double cosPsi = FMath::Cos(PsiRadians);
	double sinPsi = FMath::Sin(PsiRadians);
	double sinTheta = FMath::Sin(ThetaRadians);


	double cosThetaCosPsi = cosTheta * cosPsi;
	double cosThetaSinPsi = cosTheta * sinPsi;
	double sinCos = sinLat * cosLon;

	double b11 = -sinLon * cosThetaCosPsi + cosLon * cosThetaSinPsi;
	double b12 = -sinCos * cosThetaCosPsi - sinSin * cosThetaSinPsi - cosLat * sinTheta;
	return FMath::RadiansToDegrees(FMath::Atan2(b11, b12));
}

double UUEOpenDIS_BPFL::GetPitchFromEuler(double LatitudeRadians, double LongitudeRadians, float PsiRadians, float ThetaRadians)
{
	double sinLat = FMath::Sin(LatitudeRadians);
	double sinLon = FMath::Sin(LongitudeRadians);
	double cosLon = FMath::Cos(LongitudeRadians);
	double cosLat = FMath::Cos(LatitudeRadians);
	double cosLatCosLon = cosLat * cosLon;
	double cosLatSinLon = cosLat * sinLon;

	double cosTheta = FMath::Cos(ThetaRadians);
	double cosPsi = FMath::Cos(PsiRadians);
	double sinPsi = FMath::Sin(PsiRadians);
	double sinTheta = FMath::Sin(ThetaRadians);

	return FMath::RadiansToDegrees(FMath::Asin(cosLatCosLon * cosTheta * cosPsi + cosLatSinLon * cosTheta * sinPsi - sinLat * sinTheta));
}

double UUEOpenDIS_BPFL::GetRollFromEuler(double LatitudeRadians, double LongitudeRadians, FPsiThetaPhi PsiThetaPhiRadians)
{
	double sinLat = FMath::Sin(LatitudeRadians);
	double sinLon = FMath::Sin(LongitudeRadians);
	double cosLon = FMath::Cos(LongitudeRadians);
	double cosLat = FMath::Cos(LatitudeRadians);
	double cosLatCosLon = cosLat * cosLon;
	double cosLatSinLon = cosLat * sinLon;

	double cosTheta = FMath::Cos(PsiThetaPhiRadians.Theta);
	double sinTheta = FMath::Sin(PsiThetaPhiRadians.Theta);
	double cosPsi = FMath::Cos(PsiThetaPhiRadians.Psi);
	double sinPsi = FMath::Sin(PsiThetaPhiRadians.Psi);
	double sinPhi = FMath::Sin(PsiThetaPhiRadians.Phi);
	double cosPhi = FMath::Cos(PsiThetaPhiRadians.Phi);

	double sinPhiSinTheta = sinPhi * sinTheta;
	double cosPhiSinTheta = cosPhi * sinTheta;

	double b23 = cosLatCosLon * (-cosPhi * sinPsi + sinPhiSinTheta * cosPsi) +
		cosLatSinLon * (cosPhi * cosPsi + sinPhiSinTheta * sinPsi) +
		sinLat * (sinPhi * cosTheta);

	double b33 = cosLatCosLon * (sinPhi * sinPsi + cosPhiSinTheta * cosPsi) +
		cosLatSinLon * (-sinPhi * cosPsi + cosPhiSinTheta * sinPsi) +
		sinLat * (cosPhi * cosTheta);

	return FMath::RadiansToDegrees(FMath::Atan2(-b23, -b33));
}

void UUEOpenDIS_BPFL::EulerToENU(float LatInRad, float LonInRad, FPsiThetaPhi PsiThetaPhiRadians, FRotator& TaitBryanAnglesOut)
{
	TaitBryanAnglesOut.Pitch = GetPitchFromEuler(LatInRad, LonInRad, PsiThetaPhiRadians.Psi, PsiThetaPhiRadians.Theta);
	TaitBryanAnglesOut.Yaw = GetHeadingFromEuler(LatInRad, LonInRad, PsiThetaPhiRadians.Psi, PsiThetaPhiRadians.Theta);
	TaitBryanAnglesOut.Roll = GetRollFromEuler(LatInRad, LonInRad, PsiThetaPhiRadians);
}

void UUEOpenDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(const FEarthCenteredEarthFixedDouble ECEF,	FLatLonHeightDouble& OutLatLonHeightDegreesMeters)
{
	constexpr double EarthSemiMajorRadiusMeters = 6378137;
	constexpr double EarthSemiMinorRadiusMeters = 6356752.3142;

	const double Longitude = FMath::RadiansToDegrees(FMath::Atan2(ECEF.Y, ECEF.X));
	// Latitude accurate to ~5 decimal places
	const double Latitude = FMath::RadiansToDegrees(FMath::Atan((FMath::Square(EarthSemiMajorRadiusMeters) / FMath::Square(EarthSemiMinorRadiusMeters))*(ECEF.Z / FMath::Sqrt(FMath::Square(ECEF.X) + FMath::Square(ECEF.Y)))));

	const double EarthSemiMajorRadiusMetersSquare = FMath::Square(EarthSemiMajorRadiusMeters);
	const double EarthSemiMinorRadiusMetersSquare = FMath::Square(EarthSemiMinorRadiusMeters);
	const double DistFromXToY = FMath::Sqrt(FMath::Square(ECEF.X) + FMath::Square(ECEF.Y));
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
	FLatLonHeightDouble latLonHeightDouble;

	FEarthCenteredEarthFixedDouble ecefDouble;
	ecefDouble.X = static_cast<double>(ECEF.X);
	ecefDouble.Y = static_cast<double>(ECEF.Y);
	ecefDouble.Z = static_cast<double>(ECEF.Z);

	CalculateLatLonHeightFromEcefXYZ(ecefDouble, latLonHeightDouble);


	OutLatLonHeightDegreesMeters.Latitude = latLonHeightDouble.Latitude;
	OutLatLonHeightDegreesMeters.Longitude = latLonHeightDouble.Longitude;
	OutLatLonHeightDegreesMeters.Height = latLonHeightDouble.Height;
}

void UUEOpenDIS_BPFL::CalculateEcefXYZFromLatLonHeight(const FLatLonHeightDouble OutLatLonHeightDegreesMeters, FEarthCenteredEarthFixedDouble& OutECEF)
{
	constexpr double EarthSemiMajorRadiusMeters = 6378137;
	constexpr double EarthSemiMinorRadiusMeters = 6356752.3142;

	/*const double CosLatitude = FMath::Cos(FMath::DegreesToRadians(LatitudeDegrees));
	const double SinLatitude = FMath::Sin(FMath::DegreesToRadians(LatitudeDegrees));
	const double CosLongitude = FMath::Cos(FMath::DegreesToRadians(LongitudeDegrees));
	const double SinLongitude = FMath::Sin(FMath::DegreesToRadians(LongitudeDegrees));*/
	const double CosLatitude = glm::cos(glm::radians(OutLatLonHeightDegreesMeters.Latitude));
	const double SinLatitude = glm::sin(glm::radians(OutLatLonHeightDegreesMeters.Latitude));
	const double CosLongitude = FMath::Cos(FMath::DegreesToRadians(OutLatLonHeightDegreesMeters.Longitude));
	const double SinLongitude = FMath::Sin(FMath::DegreesToRadians(OutLatLonHeightDegreesMeters.Longitude));

	const double XYBaseConversion = (EarthSemiMajorRadiusMeters / (FMath::Sqrt(FMath::Square(CosLatitude) + ((FMath::Square(EarthSemiMinorRadiusMeters) / FMath::Square(EarthSemiMajorRadiusMeters)) * FMath::Square(SinLatitude))))) + OutLatLonHeightDegreesMeters.Height;
	const double ZBaseConversion = (EarthSemiMinorRadiusMeters / (((FMath::Sqrt(FMath::Square(CosLatitude) * (FMath::Square(EarthSemiMajorRadiusMeters) / FMath::Square(EarthSemiMinorRadiusMeters)) + FMath::Square(SinLatitude)))))) + OutLatLonHeightDegreesMeters.Height;

	DIS::Vector3Double Location;

	const double EcefX = XYBaseConversion * CosLatitude * CosLongitude;
	const double EcefY = XYBaseConversion * CosLatitude * SinLongitude;
	const double EcefZ = ZBaseConversion * SinLatitude;

	Location.setX(EcefX);
	Location.setY(EcefY);
	Location.setZ(EcefZ);

	OutECEF.X = EcefX;
	OutECEF.Y = EcefY;
	OutECEF.Z = EcefZ;
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


FMatrix UUEOpenDIS_BPFL::CreateNCrossXMatrix(FVector NVector)
{
	const auto NMatrix = FMatrix(FPlane(0, -NVector.Z, NVector.Y, 0),
		FPlane(NVector.Z, 0, -NVector.X, 0),
		FPlane(-NVector.Y, NVector.X, 0, 0),
		FPlane(0, 0, 0, 0)
	);

	return NMatrix;
}

glm::dmat3x3 UUEOpenDIS_BPFL::CreateNCrossXMatrix(glm::dvec3 NVector)
{
	return glm::dmat3x3(0, -NVector.z, NVector.y, NVector.z, 0, -NVector.x, -NVector.y, NVector.x, 0);
}

void UUEOpenDIS_BPFL::CreateRotationMatrix(const FVector AxisVector, const float ThetaRadians, FMatrix& OutRotationMatrix)
{
	const float CosTheta = FMath::Cos(ThetaRadians);
	const float SinTheta = FMath::Sin(ThetaRadians);

	OutRotationMatrix = FMatrix(FPlane(FVector(0), 0), FPlane(FVector(0), 0), FVector4(FVector(0), 0), FVector4(FVector(0), 0));

	const auto N = FMatrix(FPlane(AxisVector, 0), FPlane(FVector(0), 0), FVector4(FVector(0), 0), FVector4(FVector(0), 0));
	FMatrix NTransposeN = N.GetTransposed() * N;

	FMatrix NCrossX = CreateNCrossXMatrix(AxisVector);

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

void UUEOpenDIS_BPFL::CreateRotationMatrix(glm::dvec3 AxisVector, double ThetaRadians, glm::dmat3x3& OutRotationMatrix)
{
	const double CosTheta = glm::cos(ThetaRadians);
	const double SinTheta = glm::sin(ThetaRadians);

	const auto N = AxisVector;
	const auto NMat = glm::dmat3(N, glm::dvec3(0), glm::dvec3(0));

	const auto NTransposeN = NMat * glm::transpose(NMat);
	const auto NCrossN = CreateNCrossXMatrix(N);

	OutRotationMatrix = ((1 - CosTheta) * NTransposeN) + (CosTheta * glm::identity<glm::dmat3x3>()) + (SinTheta * NCrossN);
}

// TODO: Separate this into it's own function and an additional function for applying roll
void UUEOpenDIS_BPFL::ApplyHeadingPitchToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the X and Y vectors around the Z vector by the Heading
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.NorthVector, HeadingDegrees, NorthEastDownVectors.DownVector, OutX);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, HeadingDegrees, NorthEastDownVectors.DownVector, OutY);

	// Rotate the X and Z vectors around the Y vector by the Pitch
	RotateVectorAroundAxisByDegrees(OutX, PitchDegrees, OutY, OutX);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, PitchDegrees, OutY, OutZ);
}

// TODO: Implement this
void UUEOpenDIS_BPFL::ApplyRollToNorthEastDownVector(const float RollDegrees, const FNorthEastDown NorthEastDownVectors, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the Y and Z vectors around the X vector by the Roll
	OutX = NorthEastDownVectors.NorthVector;
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.EastVector, RollDegrees, NorthEastDownVectors.NorthVector, OutY);
	RotateVectorAroundAxisByDegrees(NorthEastDownVectors.DownVector, RollDegrees, NorthEastDownVectors.NorthVector, OutZ);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByRadians(glm::dvec3 VectorToRotate, double ThetaRadians, glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	auto RotationMatrix = glm::dmat3x3();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	OutRotatedVector = RotationMatrix * VectorToRotate;
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByDegrees(glm::dvec3 VectorToRotate, float ThetaDegrees, glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, glm::radians(ThetaDegrees), AxisVector, OutRotatedVector);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByRadians(FVector VectorToRotate, float ThetaRadians, FVector AxisVector, FVector& OutRotatedVector)
{
	const auto ZeroPlane = FPlane(FVector(0), 0);
	const auto VectorMatrix = FMatrix(FPlane(VectorToRotate, 0), ZeroPlane, ZeroPlane, ZeroPlane);
	auto RotationMatrix = FMatrix();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	FMatrix ResMatrix = VectorMatrix * RotationMatrix.GetTransposed();
	OutRotatedVector = ResMatrix.GetScaledAxis(EAxis::X);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByDegrees(FVector VectorToRotate, float ThetaDegrees, FVector AxisVector, FVector& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, FMath::DegreesToRadians(ThetaDegrees), AxisVector, OutRotatedVector);
}

// TODO: Separate into ApplyHeadingPitch... and ApplyRoll...
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
	FNorthEastDown northEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, northEastDownVectors);

	FVector X, Y, Z, X2, Y2, Z2;
	ApplyHeadingPitchRollToNorthEastDownVector(HeadingPitchRollDegrees, northEastDownVectors, X, Y, Z);

	const auto X0 = FVector(1, 0, 0);
	const auto Y0 = FVector(0, 1, 0);
	const auto Z0 = FVector(0, 0, 1);

	PsiThetaPhiDegrees.Psi = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(X, Y0), FVector::DotProduct(X, X0)));
	PsiThetaPhiDegrees.Theta = FMath::RadiansToDegrees(FMath::Atan2(-FVector::DotProduct(X, Z0), FMath::Sqrt(FMath::Square(FVector::DotProduct(X, X0)) + FMath::Square(FVector::DotProduct(X, Y0)))));

	northEastDownVectors.NorthVector = X0;
	northEastDownVectors.EastVector = Y0;
	northEastDownVectors.DownVector = Z0;

	ApplyHeadingPitchToNorthEastDownVector(PsiThetaPhiDegrees.Psi, PsiThetaPhiDegrees.Theta, northEastDownVectors, X2, Y2, Z2);

	PsiThetaPhiDegrees.Phi = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(Y, Z2), FVector::DotProduct(Y, Y2)));
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(const FHeadingPitchRoll HeadingPitchRollRadians, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FHeadingPitchRoll headingPitchRollDegrees;
	headingPitchRollDegrees.Heading = FMath::RadiansToDegrees(HeadingPitchRollRadians.Heading);
	headingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(HeadingPitchRollRadians.Pitch);
	headingPitchRollDegrees.Roll = FMath::RadiansToDegrees(HeadingPitchRollRadians.Roll);

	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, PsiThetaPhiRadians);
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(const FHeadingPitchRoll HeadingPitchRollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, FPsiThetaPhi& PsiThetaPhiRadians)
{
	FPsiThetaPhi psiThetaPhiDegrees;
	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(HeadingPitchRollDegrees, LatitudeDegrees, LongitudeDegrees, psiThetaPhiDegrees);

	PsiThetaPhiRadians.Psi = FMath::DegreesToRadians(psiThetaPhiDegrees.Psi);
	PsiThetaPhiRadians.Theta = FMath::DegreesToRadians(psiThetaPhiDegrees.Theta);
	PsiThetaPhiRadians.Phi = FMath::DegreesToRadians(psiThetaPhiDegrees.Phi);
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
	FNorthEastDown northEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, northEastDownVectors);

	const auto X0 = FVector(1, 0, 0);
	const auto Y0 = FVector(0, 1, 0);
	const auto Z0 = FVector(0, 0, 1);

	FNorthEastDown startingVectorsForRotation;
	startingVectorsForRotation.NorthVector = X0;
	startingVectorsForRotation.EastVector = Y0;
	startingVectorsForRotation.DownVector = Z0;

	FVector X3, Y3, Z3, X2, Y2, Z2;

	FHeadingPitchRoll headingPitchRoll;
	headingPitchRoll.Heading = PsiThetaPhiDegrees.Psi;
	headingPitchRoll.Pitch = PsiThetaPhiDegrees.Theta;
	headingPitchRoll.Roll = PsiThetaPhiDegrees.Phi;

	ApplyHeadingPitchRollToNorthEastDownVector(headingPitchRoll, startingVectorsForRotation, X3, Y3, Z3);

	HeadingPitchRollDegrees.Heading = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(X3, northEastDownVectors.EastVector), FVector::DotProduct(X3, northEastDownVectors.NorthVector)));
	HeadingPitchRollDegrees.Pitch = FMath::RadiansToDegrees(FMath::Atan2(-FVector::DotProduct(X3, northEastDownVectors.DownVector), FMath::Sqrt(FMath::Square(FVector::DotProduct(X3, northEastDownVectors.EastVector)) + FMath::Square(FVector::DotProduct(X3, northEastDownVectors.NorthVector)))));

	ApplyHeadingPitchToNorthEastDownVector(HeadingPitchRollDegrees.Heading, HeadingPitchRollDegrees.Pitch, northEastDownVectors, X2, Y2, Z2);
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

void UUEOpenDIS_BPFL::GetUnrealRotationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, FRotator& EntityRotation)
{
	FEarthCenteredEarthFixedDouble ecefDouble;
	ecefDouble.X = EntityStatePdu.EntityLocationDouble[0];
	ecefDouble.Y = EntityStatePdu.EntityLocationDouble[1];
	ecefDouble.Z = EntityStatePdu.EntityLocation[2];

	FLatLonHeightDouble LatLonHeightDouble;

	CalculateLatLonHeightFromEcefXYZ(ecefDouble, LatLonHeightDouble);

	FNorthEastDown NorthEastDownVectors;
	CalculateNorthEastDownVectorsFromLatLon(LatLonHeightDouble.Latitude, LatLonHeightDouble.Longitude, NorthEastDownVectors);

	FPsiThetaPhi psiThetaPhiRadians;
	psiThetaPhiRadians.Psi = EntityStatePdu.EntityOrientation.Yaw;
	psiThetaPhiRadians.Theta = EntityStatePdu.EntityOrientation.Pitch;
	psiThetaPhiRadians.Phi = EntityStatePdu.EntityOrientation.Roll;

	FHeadingPitchRoll headingPitchRoll;
	//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, LatLonHeightDouble.Latitude, LatLonHeightDouble.Longitude, headingPitchRoll);

	EntityRotation.Roll = headingPitchRoll.Roll;
	EntityRotation.Pitch = headingPitchRoll.Pitch;
	EntityRotation.Yaw = headingPitchRoll.Heading;
}

void UUEOpenDIS_BPFL::GetEntityLocationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, const FLatLonHeightFloat OriginLatLonAlt, const FNorthEastDown OriginNorthEastDown, FVector& EntityLocation, FLatLonHeightFloat& EntityLatLonHeightLocation)
{
	auto EntityLocationDouble = FEarthCenteredEarthFixedDouble(EntityStatePdu.EntityLocationDouble[0], EntityStatePdu.EntityLocationDouble[1], EntityStatePdu.EntityLocationDouble[2]);
	FLatLonHeightDouble LatLonHeightDouble;
	CalculateLatLonHeightFromEcefXYZ(EntityLocationDouble, LatLonHeightDouble);

	// Convert longitude difference to meters
	auto EntityNorthDistance = (LatLonHeightDouble.Longitude - OriginLatLonAlt.Longitude) * (10000 / 90.) * 1000;
	// Convert latitude difference to meters
	auto EntityEastDistance = (LatLonHeightDouble.Latitude - OriginLatLonAlt.Latitude) * (10000 / 90.) * 1000;
	auto EntityUpDistance = LatLonHeightDouble.Height - OriginLatLonAlt.Height;
	FVector EntityNorthVector, EntityEastVector, EntityUpVector;
	EntityNorthVector = OriginNorthEastDown.NorthVector * EntityNorthDistance;
	EntityEastVector = OriginNorthEastDown.EastVector * EntityEastDistance;
	EntityUpVector = -OriginNorthEastDown.DownVector * EntityUpDistance;

	EntityLocation = EntityNorthVector + EntityEastVector + EntityUpVector;
}

void UUEOpenDIS_BPFL::GetEntityLocationAndOrientation(const FEntityStatePDU EntityStatePdu, const FLatLonHeightFloat OriginLatLonAlt, FNorthEastDown NorthEastDownVectors, FVector& EntityLocation, FLatLonHeightFloat& EntityLatLonHeightLocation, FRotator& EntityRotation)
{
	GetEntityLocationFromEntityStatePdu(EntityStatePdu, OriginLatLonAlt, NorthEastDownVectors, EntityLocation, EntityLatLonHeightLocation);
	GetUnrealRotationFromEntityStatePdu(EntityStatePdu, EntityRotation);
}
