// Fill out your copyright notice in the Description page of Project Settings.


#include "UEOpenDIS_BPFL.h"
#include <glm/gtx/quaternion.hpp>

float UUEOpenDIS_BPFL::GetHeadingFromEuler(float Lat, float Lon, float Psi, float Theta)
{
	double sinlat = FMath::Sin(Lat);
	double sinlon = FMath::Sin(Lon);
	double coslon = FMath::Cos(Lon);
	double coslat = FMath::Cos(Lat);
	double sinsin = sinlat * sinlon;

	double cosTheta = FMath::Cos(Theta);
	double cosPsi = FMath::Cos(Psi);
	double sinPsi = FMath::Sin(Psi);
	double sinTheta = FMath::Sin(Theta);


	double cosThetaCosPsi = cosTheta * cosPsi;
	double cosThetaSinPsi = cosTheta * sinPsi;
	double sincos = sinlat * coslon;

	double b11 = -sinlon * cosThetaCosPsi + coslon * cosThetaSinPsi;
	double b12 = -sincos * cosThetaCosPsi - sinsin * cosThetaSinPsi - coslat * sinTheta;
	return FMath::RadiansToDegrees(FMath::Atan2(b11, b12));
}

float UUEOpenDIS_BPFL::GetPitchFromEuler(float Lat, float Lon, float Psi, float Theta)
{
	double sinlat = FMath::Sin(Lat);
	double sinlon = FMath::Sin(Lon);
	double coslon = FMath::Cos(Lon);
	double coslat = FMath::Cos(Lat);
	double cosLatCosLon = coslat * coslon;
	double cosLatSinLon = coslat * sinlon;

	double cosTheta = FMath::Cos(Theta);
	double cosPsi = FMath::Cos(Psi);
	double sinPsi = FMath::Sin(Psi);
	double sinTheta = FMath::Sin(Theta);

	return FMath::RadiansToDegrees(FMath::Asin(cosLatCosLon * cosTheta * cosPsi + cosLatSinLon * cosTheta * sinPsi - sinlat * sinTheta));
}

float UUEOpenDIS_BPFL::GetRollFromEuler(float Lat, float Lon, float Psi, float Theta, float Phi)
{
	double sinlat = FMath::Sin(Lat);
	double sinlon = FMath::Sin(Lon);
	double coslon = FMath::Cos(Lon);
	double coslat = FMath::Cos(Lat);
	double cosLatCosLon = coslat * coslon;
	double cosLatSinLon = coslat * sinlon;

	double cosTheta = FMath::Cos(Theta);
	double sinTheta = FMath::Sin(Theta);
	double cosPsi = FMath::Cos(Psi);
	double sinPsi = FMath::Sin(Psi);
	double sinPhi = FMath::Sin(Phi);
	double cosPhi = FMath::Cos(Phi);

	double sinPhiSinTheta = sinPhi * sinTheta;
	double cosPhiSinTheta = cosPhi * sinTheta;

	double b23 = cosLatCosLon * (-cosPhi * sinPsi + sinPhiSinTheta * cosPsi) +
		cosLatSinLon * (cosPhi * cosPsi + sinPhiSinTheta * sinPsi) +
		sinlat * (sinPhi * cosTheta);

	double b33 = cosLatCosLon * (sinPhi * sinPsi + cosPhiSinTheta * cosPsi) +
		cosLatSinLon * (-sinPhi * cosPsi + cosPhiSinTheta * sinPsi) +
		sinlat * (cosPhi * cosTheta);

	return FMath::RadiansToDegrees(FMath::Atan2(-b23, -b33));
}

double UUEOpenDIS_BPFL::GetHeadingFromEulerDouble(double Lat, double Lon, float Psi, float Theta)
{
	double sinlat = FMath::Sin(Lat);
	double sinlon = FMath::Sin(Lon);
	double coslon = FMath::Cos(Lon);
	double coslat = FMath::Cos(Lat);
	double sinsin = sinlat * sinlon;

	double cosTheta = FMath::Cos(Theta);
	double cosPsi = FMath::Cos(Psi);
	double sinPsi = FMath::Sin(Psi);
	double sinTheta = FMath::Sin(Theta);


	double cosThetaCosPsi = cosTheta * cosPsi;
	double cosThetaSinPsi = cosTheta * sinPsi;
	double sincos = sinlat * coslon;

	double b11 = -sinlon * cosThetaCosPsi + coslon * cosThetaSinPsi;
	double b12 = -sincos * cosThetaCosPsi - sinsin * cosThetaSinPsi - coslat * sinTheta;
	return FMath::RadiansToDegrees(FMath::Atan2(b11, b12));
}

double UUEOpenDIS_BPFL::GetPitchFromEulerDouble(double Lat, double Lon, float Psi, float Theta)
{
	double sinlat = FMath::Sin(Lat);
	double sinlon = FMath::Sin(Lon);
	double coslon = FMath::Cos(Lon);
	double coslat = FMath::Cos(Lat);
	double cosLatCosLon = coslat * coslon;
	double cosLatSinLon = coslat * sinlon;

	double cosTheta = FMath::Cos(Theta);
	double cosPsi = FMath::Cos(Psi);
	double sinPsi = FMath::Sin(Psi);
	double sinTheta = FMath::Sin(Theta);

	return FMath::RadiansToDegrees(FMath::Asin(cosLatCosLon * cosTheta * cosPsi + cosLatSinLon * cosTheta * sinPsi - sinlat * sinTheta));
}

double UUEOpenDIS_BPFL::GetRollFromEulerDouble(double Lat, double Lon, float Psi, float Theta, float Phi)
{
	double sinlat = FMath::Sin(Lat);
	double sinlon = FMath::Sin(Lon);
	double coslon = FMath::Cos(Lon);
	double coslat = FMath::Cos(Lat);
	double cosLatCosLon = coslat * coslon;
	double cosLatSinLon = coslat * sinlon;

	double cosTheta = FMath::Cos(Theta);
	double sinTheta = FMath::Sin(Theta);
	double cosPsi = FMath::Cos(Psi);
	double sinPsi = FMath::Sin(Psi);
	double sinPhi = FMath::Sin(Phi);
	double cosPhi = FMath::Cos(Phi);

	double sinPhiSinTheta = sinPhi * sinTheta;
	double cosPhiSinTheta = cosPhi * sinTheta;

	double b23 = cosLatCosLon * (-cosPhi * sinPsi + sinPhiSinTheta * cosPsi) +
		cosLatSinLon * (cosPhi * cosPsi + sinPhiSinTheta * sinPsi) +
		sinlat * (sinPhi * cosTheta);

	double b33 = cosLatCosLon * (sinPhi * sinPsi + cosPhiSinTheta * cosPsi) +
		cosLatSinLon * (-sinPhi * cosPsi + cosPhiSinTheta * sinPsi) +
		sinlat * (cosPhi * cosTheta);

	return FMath::RadiansToDegrees(FMath::Atan2(-b23, -b33));
}

void UUEOpenDIS_BPFL::EulerToENU(float LatInRad, float LonInRad, float Psi, float Theta, float Phi, FRotator& TaitBryanAnglesOut)
{
	TaitBryanAnglesOut.Pitch = GetPitchFromEuler(LatInRad, LonInRad, Psi, Theta);
	TaitBryanAnglesOut.Yaw = GetHeadingFromEuler(LatInRad, LonInRad, Psi, Theta);
	TaitBryanAnglesOut.Roll = GetRollFromEuler(LatInRad, LonInRad, Psi, Theta, Phi);
}

void UUEOpenDIS_BPFL::ECEF2UE4LocationESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LocationOut)
{
	glm::dvec3 inPosDouble = glm::dvec3(EntityStatePDUIn.EntityLocationDouble[0], EntityStatePDUIn.EntityLocationDouble[1], EntityStatePDUIn.EntityLocationDouble[2]);

	// TODO: Update below line to perform conversions without need of CesiumGeoreference
	glm::dvec3 outposDouble = glm::dvec3(0, 0, 0);//CesiumGeoref->TransformEcefToUnreal(inPosDouble);

	LocationOut[0] = outposDouble.x;
	LocationOut[1] = outposDouble.y;
	LocationOut[2] = outposDouble.z;
}

void UUEOpenDIS_BPFL::ECEF2LongLatHeightESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LonLatHeight)
{
	glm::dvec3 inPosDouble = glm::dvec3(EntityStatePDUIn.EntityLocationDouble[0], EntityStatePDUIn.EntityLocationDouble[1], EntityStatePDUIn.EntityLocationDouble[2]);

	// TODO: Update below line to perform conversions without need of CesiumGeoreference
	glm::dvec3 outposDouble = glm::dvec3(0, 0, 0);// CesiumGeoref->TransformEcefToLongitudeLatitudeHeight(inPosDouble);

	LonLatHeight[0] = outposDouble.x;
	LonLatHeight[1] = outposDouble.y;
	LonLatHeight[2] = outposDouble.z;
}

void UUEOpenDIS_BPFL::ECEF2ENU2UERotESPDU(FEntityStatePDU EntityStatePDUIn, FRotator& RotationOut)
{
	glm::dvec3 inPosDouble = glm::dvec3(EntityStatePDUIn.EntityLocationDouble[0], EntityStatePDUIn.EntityLocationDouble[1], EntityStatePDUIn.EntityLocationDouble[2]);

	// TODO: Update below line to perform conversions without need of CesiumGeoreference
	glm::dvec3 outLonLatHeight = glm::dvec3(0, 0, 0);//CesiumGeoref->TransformEcefToLongitudeLatitudeHeight(inPosDouble);

	double longitude = FMath::DegreesToRadians(outLonLatHeight.x);
	double latitude = FMath::DegreesToRadians(outLonLatHeight.y);


	RotationOut.Pitch = GetPitchFromEuler(latitude, longitude, EntityStatePDUIn.EntityOrientation.Roll, EntityStatePDUIn.EntityOrientation.Pitch);
	RotationOut.Yaw = GetHeadingFromEuler(latitude, longitude, EntityStatePDUIn.EntityOrientation.Roll, EntityStatePDUIn.EntityOrientation.Pitch);
	RotationOut.Roll = GetRollFromEuler(latitude, longitude, EntityStatePDUIn.EntityOrientation.Roll, EntityStatePDUIn.EntityOrientation.Pitch, EntityStatePDUIn.EntityOrientation.Yaw);
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

// TODO: Separate this into it's own function and an additional function for applying roll
void UUEOpenDIS_BPFL::ApplyHeadingPitchToNorthEastDownVector(const float Heading, const float Pitch,
	const FVector NorthVector, const FVector EastVector, const FVector DownVector, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	ApplyHeadingPitchRollToNorthEastDownVector(Heading, Pitch, 0, NorthVector, EastVector, DownVector, OutX, OutY, OutZ);
}

// TODO: Implement this
void UUEOpenDIS_BPFL::ApplyRollToNorthEastDownVector(const float Roll, const FVector East,
	const FVector North, const FVector Up, FVector& OutX, FVector& OutY, FVector& OutZ)
{
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByRadians(FVector VectorToRotate, float ThetaRadians, FVector AxisVector,
	FVector& OutRotatedVector)
{
	const auto ZeroPlane = FPlane(FVector(0), 0);
	const auto VectorMatrix = FMatrix(FPlane(VectorToRotate, 0), ZeroPlane, ZeroPlane, ZeroPlane);
	auto RotationMatrix = FMatrix();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	FMatrix ResMatrix = VectorMatrix * RotationMatrix.GetTransposed();
	OutRotatedVector = ResMatrix.GetScaledAxis(EAxis::X);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByDegrees(FVector VectorToRotate, float ThetaDegrees,
	FVector AxisVector, FVector& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, FMath::DegreesToRadians(ThetaDegrees), AxisVector, OutRotatedVector);
}

// TODO: Separate into ApplyHeadingPitch... and ApplyRoll...
void UUEOpenDIS_BPFL::ApplyHeadingPitchRollToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const float RollDegrees,
	const FVector NorthVector, const FVector EastVector, const FVector DownVector, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	OutX = NorthVector;
	OutY = EastVector;
	OutZ = DownVector;

	// Rotate the X and Y vectors around the Z vector by the Heading
	RotateVectorAroundAxisByDegrees(OutX, HeadingDegrees, OutZ, OutX);
	RotateVectorAroundAxisByDegrees(OutY, HeadingDegrees, OutZ, OutY);

	// Rotate the X and Z vectors around the Y vector by the Pitch
	RotateVectorAroundAxisByDegrees(OutX, PitchDegrees, OutY, OutX);
	RotateVectorAroundAxisByDegrees(OutZ, PitchDegrees, OutY, OutZ);

	// Rotate the Y and Z vectors around the X vector by the Roll
	RotateVectorAroundAxisByDegrees(OutY, RollDegrees, OutX, OutY);
	RotateVectorAroundAxisByDegrees(OutZ, RollDegrees, OutX, OutZ);
}

void UUEOpenDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(const float LatitudeDegrees, const float LongitudeDegrees,
	FVector& NorthVector, FVector& EastVector, FVector& DownVector)
{
	NorthVector = FVector(0, 0, 1);
	EastVector = FVector(0, 1, 0);
	DownVector = FVector(-1, 0, 0);

	RotateVectorAroundAxisByDegrees(EastVector, LongitudeDegrees, NorthVector, EastVector);
	RotateVectorAroundAxisByDegrees(DownVector, LongitudeDegrees, NorthVector, DownVector);

	RotateVectorAroundAxisByDegrees(NorthVector, LatitudeDegrees, -EastVector, NorthVector);
	RotateVectorAroundAxisByDegrees(DownVector, LatitudeDegrees, -EastVector, DownVector);
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(const float HeadingDegrees,
	const float PitchDegrees, const float RollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiDegrees,
	float& ThetaDegrees, float& PhiDegrees)
{
	FVector EastVector, NorthVector, DownVector;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, NorthVector, EastVector, DownVector);

	FVector X, Y, Z, X2, Y2, Z2;
	ApplyHeadingPitchRollToNorthEastDownVector(HeadingDegrees, PitchDegrees, RollDegrees, NorthVector, EastVector, DownVector, X, Y, Z);

	const auto X0 = FVector(1, 0, 0);
	const auto Y0 = FVector(0, 1, 0);
	const auto Z0 = FVector(0, 0, 1);

	PsiDegrees = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(X, Y0), FVector::DotProduct(X, X0)));
	ThetaDegrees = FMath::RadiansToDegrees(FMath::Atan2(-FVector::DotProduct(X, Z0), FMath::Sqrt(FMath::Square(FVector::DotProduct(X, X0)) + FMath::Square(FVector::DotProduct(X, Y0)))));
	ApplyHeadingPitchToNorthEastDownVector(PsiDegrees, ThetaDegrees, X0, Y0, Z0, X2, Y2, Z2);
	PhiDegrees = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(Y, Z2), FVector::DotProduct(Y, Y2)));
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(
	const float HeadingRadians, const float PitchRadians, const float RollRadians, const float LatitudeDegrees,
	const float LongitudeDegrees, float& PsiRadians, float& ThetaRadians, float& PhiRadians)
{
	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(FMath::RadiansToDegrees(HeadingRadians), FMath::RadiansToDegrees(PitchRadians), FMath::RadiansToDegrees(RollRadians), LatitudeDegrees, LongitudeDegrees, PsiRadians, ThetaRadians, PhiRadians);
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(
	const float HeadingDegrees, const float PitchDegrees, const float RollDegrees, const float LatitudeDegrees,
	const float LongitudeDegrees, float& PsiRadians, float& ThetaRadians, float& PhiRadians)
{
	float PsiDegrees, ThetaDegrees, PhiDegrees;
	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(HeadingDegrees, PitchDegrees, RollDegrees, LatitudeDegrees, LongitudeDegrees, PsiDegrees, ThetaDegrees, PhiDegrees);

	PsiRadians = FMath::DegreesToRadians(PsiDegrees);
	ThetaRadians = FMath::DegreesToRadians(ThetaDegrees);
	PhiRadians = FMath::DegreesToRadians(PhiDegrees);
}

void UUEOpenDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(
	const float HeadingRadians, const float PitchRadians, const float RollRadians, const float LatitudeDegrees,
	const float LongitudeDegrees, float& PsiDegrees, float& ThetaDegrees, float& PhiDegrees)
{
	CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(FMath::RadiansToDegrees(HeadingRadians), FMath::RadiansToDegrees(PitchRadians), FMath::RadiansToDegrees(RollRadians), LatitudeDegrees, LongitudeDegrees, PsiDegrees, ThetaDegrees, PhiDegrees);
}

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(const float PsiDegrees, const float ThetaDegrees,
	const float PhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingDegrees, float& PitchDegrees,
	float& RollDegrees)
{
	FVector EastVector, NorthVector, DownVector;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, NorthVector, EastVector, DownVector);

	const auto X0 = FVector(1, 0, 0);
	const auto Y0 = FVector(0, 1, 0);
	const auto Z0 = FVector(0, 0, 1);
	FVector X3, Y3, Z3, X2, Y2, Z2;
	ApplyHeadingPitchRollToNorthEastDownVector(PsiDegrees, ThetaDegrees, PhiDegrees, X0, Y0, Z0, X3, Y3, Z3);

	HeadingDegrees = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(X3, EastVector), FVector::DotProduct(X3, NorthVector)));
	PitchDegrees = FMath::RadiansToDegrees(FMath::Atan2(-FVector::DotProduct(X3, DownVector), FMath::Sqrt(FMath::Square(FVector::DotProduct(X3, EastVector)) + FMath::Square(FVector::DotProduct(X3, NorthVector)))));
	ApplyHeadingPitchToNorthEastDownVector(HeadingDegrees, PitchDegrees, NorthVector, EastVector, DownVector, X2, Y2, Z2);
	RollDegrees = FMath::RadiansToDegrees(FMath::Atan2(FVector::DotProduct(Y3, Z2), FVector::DotProduct(Y3, Y2)));
}

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(const float PsiRadians,
	const float ThetaRadians, const float PhiRadians, const float LatitudeDegrees, const float LongitudeDegrees,
	float& HeadingRadians, float& PitchRadians, float& RollRadians)
{
	CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(FMath::RadiansToDegrees(PsiRadians), FMath::RadiansToDegrees(ThetaRadians), FMath::RadiansToDegrees(PhiRadians), LatitudeDegrees, LongitudeDegrees, HeadingRadians, PitchRadians, RollRadians);
}

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(const float PsiRadians,
	const float ThetaRadians, const float PhiRadians, const float LatitudeDegrees, const float LongitudeDegrees,
	float& HeadingDegrees, float& PitchDegrees, float& RollDegrees)
{
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(FMath::RadiansToDegrees(PsiRadians), FMath::RadiansToDegrees(ThetaRadians), FMath::RadiansToDegrees(PhiRadians), LatitudeDegrees, LongitudeDegrees, HeadingDegrees, PitchDegrees, RollDegrees);
}

void UUEOpenDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(const float PsiDegrees,
	const float ThetaDegrees, const float PhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees,
	float& HeadingRadians, float& PitchRadians, float& RollRadians)
{
	float HeadingDegrees, PitchDegrees, RollDegrees;
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(PsiDegrees, ThetaDegrees, PhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingDegrees, PitchDegrees, RollDegrees);
	HeadingRadians = FMath::DegreesToRadians(HeadingDegrees);
	PitchRadians = FMath::DegreesToRadians(PitchDegrees);
	RollRadians = FMath::DegreesToRadians(RollDegrees);
}
