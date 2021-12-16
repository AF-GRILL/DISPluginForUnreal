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
