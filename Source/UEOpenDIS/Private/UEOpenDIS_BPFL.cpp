// Fill out your copyright notice in the Description page of Project Settings.


#include "UEOpenDIS_BPFL.h"

#include "Kismet/KismetMathLibrary.h"

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

void UUEOpenDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(const double EcefX, const double EcefY, const double EcefZ,
	double& OutLatitudeDegrees, double& OutLongitudeDegrees, double& OutHeightMeters)
{
	constexpr double EarthSemiMajorRadiusMeters = 6378137;
	constexpr double EarthSemiMinorRadiusMeters = 6356752.3142;

	const double Longitude = FMath::RadiansToDegrees(FMath::Atan2(EcefY, EcefX));
	// Latitude accurate to ~5 decimal places
	const double Latitude = FMath::RadiansToDegrees(FMath::Atan((FMath::Square(EarthSemiMajorRadiusMeters) / FMath::Square(EarthSemiMinorRadiusMeters))*(EcefZ / FMath::Sqrt(FMath::Square(EcefX) + FMath::Square(EcefY)))));

	const double EarthSemiMajorRadiusMetersSquare = FMath::Square(EarthSemiMajorRadiusMeters);
	const double EarthSemiMinorRadiusMetersSquare = FMath::Square(EarthSemiMinorRadiusMeters);
	const double DistFromXToY = FMath::Sqrt(FMath::Square(EcefX) + FMath::Square(EcefY));
	const double CosLatitude = FMath::Cos(FMath::DegreesToRadians(Latitude));
	const double SinLatitude = FMath::Sin(FMath::DegreesToRadians(Latitude));
	const double Height = (DistFromXToY / CosLatitude) - (EarthSemiMajorRadiusMetersSquare / FMath::Sqrt(
		(EarthSemiMajorRadiusMetersSquare * FMath::Square(CosLatitude)) + (EarthSemiMinorRadiusMetersSquare *
			FMath::Square(SinLatitude))));

	OutLatitudeDegrees = Latitude;
	OutLongitudeDegrees = Longitude;
	OutHeightMeters = Height;
}

void UUEOpenDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(const float EcefX, const float EcefY, const float EcefZ,
                                                       float& OutLatitudeDegrees, float& OutLongitudeDegrees, float& OutHeightMeters)
{
	double OutLatitudeDegreesDouble, OutLongitudeDegreesDouble, OutHeightMetersDouble;
	CalculateLatLonHeightFromEcefXYZ(static_cast<double>(EcefX), static_cast<double>(EcefY), static_cast<double>(EcefZ), OutLatitudeDegreesDouble, OutLongitudeDegreesDouble, OutHeightMetersDouble);

	OutLatitudeDegrees = OutLatitudeDegreesDouble;
	OutLongitudeDegrees = OutLongitudeDegreesDouble;
	OutHeightMeters = OutHeightMetersDouble;
}

void UUEOpenDIS_BPFL::CalculateEcefXYZFromLatLonHeight(const double LatitudeDegrees, const double LongitudeDegrees,
	const double HeightMeters, double& OutEcefX, double& OutEcefY, double& OutEcefZ)
{
	constexpr double EarthSemiMajorRadiusMeters = 6378137;
	constexpr double EarthSemiMinorRadiusMeters = 6356752.3142;

	/*const double CosLatitude = FMath::Cos(FMath::DegreesToRadians(LatitudeDegrees));
	const double SinLatitude = FMath::Sin(FMath::DegreesToRadians(LatitudeDegrees));
	const double CosLongitude = FMath::Cos(FMath::DegreesToRadians(LongitudeDegrees));
	const double SinLongitude = FMath::Sin(FMath::DegreesToRadians(LongitudeDegrees));*/
	const double CosLatitude = glm::cos(glm::radians(LatitudeDegrees));
	const double SinLatitude = glm::sin(glm::radians(LatitudeDegrees));
	const double CosLongitude = FMath::Cos(FMath::DegreesToRadians(LongitudeDegrees));
	const double SinLongitude = FMath::Sin(FMath::DegreesToRadians(LongitudeDegrees));

	const double XYBaseConversion = (EarthSemiMajorRadiusMeters / (FMath::Sqrt(FMath::Square(CosLatitude) + ((FMath::Square(EarthSemiMinorRadiusMeters) / FMath::Square(EarthSemiMajorRadiusMeters)) * FMath::Square(SinLatitude))))) + HeightMeters;
	const double ZBaseConversion = (EarthSemiMinorRadiusMeters / (((FMath::Sqrt(FMath::Square(CosLatitude) * (FMath::Square(EarthSemiMajorRadiusMeters) / FMath::Square(EarthSemiMinorRadiusMeters)) + FMath::Square(SinLatitude)))))) + HeightMeters;

	DIS::Vector3Double Location;

	const double EcefX = XYBaseConversion * CosLatitude * CosLongitude;
	const double EcefY = XYBaseConversion * CosLatitude * SinLongitude;
	const double EcefZ = ZBaseConversion * SinLatitude;

	Location.setX(EcefX);
	Location.setY(EcefY);
	Location.setZ(EcefZ);

	OutEcefX = EcefX;
	OutEcefY = EcefY;
	OutEcefZ = EcefZ;
}

void UUEOpenDIS_BPFL::CalculateEcefXYZFromLatLonHeight(const float LatitudeDegrees, const float LongitudeDegrees,
                                                       const float HeightMeters, float& OutEcefX, float& OutEcefY, float& OutEcefZ)
{
	double OutEcefXDouble, OutEcefYDouble, OutEcefZDouble;
	CalculateEcefXYZFromLatLonHeight(static_cast<double>(LatitudeDegrees), static_cast<double>(LongitudeDegrees), static_cast<double>(HeightMeters), OutEcefXDouble, OutEcefYDouble, OutEcefZDouble);

	OutEcefX = OutEcefXDouble;
	OutEcefY = OutEcefYDouble;
	OutEcefZ = OutEcefZDouble;
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
void UUEOpenDIS_BPFL::ApplyHeadingPitchToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees,
	const FVector NorthVector, const FVector EastVector, const FVector DownVector, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the X and Y vectors around the Z vector by the Heading
	RotateVectorAroundAxisByDegrees(NorthVector, HeadingDegrees, DownVector, OutX);
	RotateVectorAroundAxisByDegrees(EastVector, HeadingDegrees, DownVector, OutY);

	// Rotate the X and Z vectors around the Y vector by the Pitch
	RotateVectorAroundAxisByDegrees(OutX, PitchDegrees, OutY, OutX);
	RotateVectorAroundAxisByDegrees(DownVector, PitchDegrees, OutY, OutZ);
}

// TODO: Implement this
void UUEOpenDIS_BPFL::ApplyRollToNorthEastDownVector(const float RollDegrees, const FVector North,
	const FVector East, const FVector Down, FVector& OutX, FVector& OutY, FVector& OutZ)
{
	// Rotate the Y and Z vectors around the X vector by the Roll
	RotateVectorAroundAxisByDegrees(East, RollDegrees, North, OutY);
	RotateVectorAroundAxisByDegrees(Down, RollDegrees, North, OutZ);
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByRadians(glm::dvec3 VectorToRotate, double ThetaRadians, glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	auto RotationMatrix = glm::dmat3x3();
	CreateRotationMatrix(AxisVector, ThetaRadians, RotationMatrix);
	OutRotatedVector = RotationMatrix * VectorToRotate;
}

void UUEOpenDIS_BPFL::RotateVectorAroundAxisByDegrees(glm::dvec3 VectorToRotate, float ThetaDegrees,
	glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector)
{
	RotateVectorAroundAxisByRadians(VectorToRotate, glm::radians(ThetaDegrees), AxisVector, OutRotatedVector);
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

	ApplyHeadingPitchToNorthEastDownVector(HeadingDegrees, PitchDegrees, OutX, OutY, OutZ, OutX, OutY, OutZ);

	ApplyRollToNorthEastDownVector(RollDegrees, OutX, OutY, OutZ, OutX, OutY, OutZ);
}

void UUEOpenDIS_BPFL::CalculateNorthEastDownVectorsFromLatLon(const float LatitudeDegrees, const float LongitudeDegrees,
	FVector& NorthVector, FVector& EastVector, FVector& DownVector)
{
	NorthVector = FVector::ZAxisVector;
	EastVector = FVector::YAxisVector;
	DownVector = -FVector::XAxisVector;

	RotateVectorAroundAxisByDegrees(EastVector, LongitudeDegrees, NorthVector, EastVector);
	RotateVectorAroundAxisByDegrees(DownVector, LongitudeDegrees, NorthVector, DownVector);

	RotateVectorAroundAxisByDegrees(NorthVector, LatitudeDegrees, -EastVector, NorthVector);
	RotateVectorAroundAxisByDegrees(DownVector, LatitudeDegrees, -EastVector, DownVector);
}

void UUEOpenDIS_BPFL::CalculateLatLongFromNorthEastDownVectors(const FVector NorthVector, const FVector EastVector,
	const FVector DownVector, float& LatitudeDegrees, float& LongitudeDegrees)
{
	LongitudeDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::YAxisVector, EastVector) / EastVector.Size()));
	LatitudeDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FVector::ZAxisVector, NorthVector) / NorthVector.Size()));
	
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
	CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(FMath::RadiansToDegrees(HeadingRadians), FMath::RadiansToDegrees(PitchRadians), FMath::RadiansToDegrees(RollRadians), LatitudeDegrees, LongitudeDegrees, PsiDegrees, ThetaDegrees, PhiDegrees);
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
	// Unsure about this section
	/*if (FMath::IsNearlyEqual(HeadingDegrees, 30, 1E-3f))
	{
		HeadingDegrees = 0;
	}if (FMath::IsNearlyEqual(PitchDegrees, 30, 1E-3f))
	{
		PitchDegrees = 0;
	}*/
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
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(PsiDegrees, ThetaDegrees, PhiDegrees, LatitudeDegrees, LongitudeDegrees, HeadingDegrees, PitchDegrees, RollDegrees);
	HeadingRadians = FMath::DegreesToRadians(HeadingDegrees);
	PitchRadians = FMath::DegreesToRadians(PitchDegrees);
	RollRadians = FMath::DegreesToRadians(RollDegrees);
}

void UUEOpenDIS_BPFL::GetUnrealRotationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, FVector NorthVector, FVector EastVector, FVector DownVector, FRotator& EntityRotation)
{
	double LatitudeDegrees, LongitudeDegrees, HeightMeters;
	CalculateLatLonHeightFromEcefXYZ(EntityStatePdu.EntityLocationDouble[0], EntityStatePdu.EntityLocationDouble[1],
	                                 EntityStatePdu.EntityLocation[2], LatitudeDegrees, LongitudeDegrees, HeightMeters);

	FVector LocalNorthVector, LocalEastVector, LocalDownVector;
	CalculateNorthEastDownVectorsFromLatLon(LatitudeDegrees, LongitudeDegrees, LocalNorthVector, LocalEastVector, LocalDownVector);
	

	float HeadingDegrees, PitchDegrees, RollDegrees;
	//NOTE: Roll=Phi, Pitch=Theta, Yaw=Psi
	CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(EntityStatePdu.EntityOrientation.Yaw,
	                                                               EntityStatePdu.EntityOrientation.Pitch,
	                                                               EntityStatePdu.EntityOrientation.Roll,
	                                                               LatitudeDegrees, LongitudeDegrees, HeadingDegrees,
	                                                               PitchDegrees, RollDegrees);

	/*RotateVectorAroundAxisByDegrees(LocalNorthVector, HeadingDegrees, LocalDownVector, LocalNorthVector);
	RotateVectorAroundAxisByDegrees(LocalEastVector, HeadingDegrees, LocalDownVector, LocalEastVector);

	RotateVectorAroundAxisByDegrees(LocalNorthVector, PitchDegrees, LocalEastVector, LocalNorthVector);
	RotateVectorAroundAxisByDegrees(LocalDownVector, PitchDegrees, LocalEastVector, LocalDownVector);

	RotateVectorAroundAxisByDegrees(LocalEastVector, RollDegrees, LocalNorthVector, LocalEastVector);
	RotateVectorAroundAxisByDegrees(LocalDownVector, RollDegrees, LocalNorthVector, LocalDownVector);*/
	FRotator HeadingRotator = FRotator(0, HeadingDegrees, 0);
	FRotator PitchRotator = FRotator(PitchDegrees, 0, 0);
	FRotator RollRotator = FRotator(0, 0, RollDegrees);

	EntityRotation = UKismetMathLibrary::ComposeRotators(UKismetMathLibrary::ComposeRotators(UKismetMathLibrary::ComposeRotators(RollRotator, PitchRotator), HeadingRotator), UKismetMathLibrary::MakeRotationFromAxes(NorthVector, EastVector, -DownVector));
	/*const FMatrix RotationMatrix(NorthVector, EastVector, -DownVector, FVector::ZeroVector);
	EntityRotation = RotationMatrix.Rotator();*/
}

void UUEOpenDIS_BPFL::GetEntityLocationFromEntityStatePdu(const FEntityStatePDU EntityStatePdu, FVector& EntityLocation)
{
	auto EntityLocationDouble = EntityStatePdu.EntityLocationDouble;
	double LatitudeDegrees, LongitudeDegrees, HeightMeters;
	CalculateLatLonHeightFromEcefXYZ(EntityLocationDouble[0], EntityLocationDouble[1], EntityLocationDouble[2], LatitudeDegrees, LongitudeDegrees, HeightMeters);

	EntityLocation = FVector(LatitudeDegrees, LongitudeDegrees, HeightMeters);
}

void UUEOpenDIS_BPFL::GetEntityLocationAndOrientation(const FEntityStatePDU EntityStatePdu, FVector NorthVector,
	FVector EastVector, FVector DownVector, FVector& EntityLocation, FRotator& EntityRotation)
{
	GetEntityLocationFromEntityStatePdu(EntityStatePdu, EntityLocation);
	GetUnrealRotationFromEntityStatePdu(EntityStatePdu, NorthVector, EastVector, DownVector, EntityRotation);
}
