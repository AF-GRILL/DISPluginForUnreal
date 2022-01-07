// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include <glm/gtx/quaternion.hpp>
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UEOpenDIS_BPFL.generated.h"

/**
 * 
 */
UCLASS()
class UEOPENDIS_API UUEOpenDIS_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	/**
	 * Creates a 4x4 n^x matrix used for creating a rotation matrix
	 * @param NVector A 3x1 vector representing the axis of rotation
	 */
	static FMatrix CreateNCrossXMatrix(FVector NVector);

	/**
	 * Creates a 3x3 n^x matrix used for creating a rotation matrix
	 * @param NVector A 3x1 vector representing the axis of rotation
	 */
	static glm::dmat3x3 CreateNCrossXMatrix(glm::dvec3 NVector);

	/**
	 * Creates a 4x4 rotation matrix around the given axis of rotation rotating by Theta degrees
	 * @param AxisVector A 3x1 vector representing the axis of rotation
	 * @param ThetaRadians The amount to rotate given in radians
	 * @param OutRotationMatrix
	 */
	static void CreateRotationMatrix(FVector AxisVector, float ThetaRadians, FMatrix& OutRotationMatrix);

	/**
	 * Creates a 3x3 rotation matrix around the given axis of rotation rotating by Theta degrees
	 * @param AxisVector A 3x1 vector representing the axis of rotation
	 * @param ThetaRadians The amount to rotate given in radians
	 * @param OutRotationMatrix
	 */
	static void CreateRotationMatrix(glm::dvec3 AxisVector, double ThetaRadians, glm::dmat3x3& OutRotationMatrix);

	/**
	 * Rotates the given East, North, and Up vectors by the given Heading and Pitch
	 * @param HeadingDegrees The degrees from North of the facing direction (spin left and right)
	 * @param PitchDegrees The degrees rotated about the local Y axis (front tip up and down)
	 * @param NorthVector The vector pointing to the North
	 * @param EastVector The vector pointing toward the East
	 * @param DownVector The vector pointing away from the center of the Earth
	 * @param OutX The x axis (forward) vector with the heading and pitch applied
	 * @param OutY The y axis (right) vector with the heading and pitch applied
	 * @param OutZ the z axis (up) vector with the heading and pitch applied
	 */
	static void ApplyHeadingPitchToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const FVector NorthVector, const FVector EastVector, const FVector DownVector, FVector& OutX, FVector& OutY, FVector& OutZ);

	/**
	 * Rotates the given East, North, and Up vectors by the given Heading and Pitch
	 * @param RollDegrees The degrees rotated about the local X axis (tilt left and right)
	 * @param North The vector pointing toward the North
	 * @param East The vector pointing to the East
	 * @param Down The vector pointing toward the center of the Earth
	 * @param OutX The x axis (forward) vector with the heading and pitch applied
	 * @param OutY The y axis (right) vector with the heading and pitch applied
	 * @param OutZ the z axis (up) vector with the heading and pitch applied
	 */
	static void ApplyRollToNorthEastDownVector(const float RollDegrees, const FVector North, const FVector East, const FVector Down, FVector& OutX, FVector& OutY, FVector& OutZ);

public:
	static float GetHeadingFromEuler(float Lat, float Lon, float Psi, float Theta);
	static float GetPitchFromEuler(float Lat, float Lon, float Psi, float Theta);
	static float GetRollFromEuler(float Lat, float Lon, float Psi, float Theta, float Phi);
	static double GetHeadingFromEulerDouble(double Lat, double Lon, float Psi, float Theta);
	static double GetPitchFromEulerDouble(double Lat, double Lon, float Psi, float Theta);
	static double GetRollFromEulerDouble(double Lat, double Lon, float Psi, float Theta, float Phi);

	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void EulerToENU(float LatInRad, float LonInRad, float Psi, float Theta, float Phi, FRotator& TaitBryanAnglesOut);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void ECEF2UE4LocationESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LocationOut);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void ECEF2LongLatHeightESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LonLatHeight);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void ECEF2ENU2UERotESPDU(FEntityStatePDU EntityStatePDUIn, FRotator& RotationOut);

	/**
	 * Converts DIS X, Y, Z coordinates (ECEF) to Latitude, Longitude, and Height (LLH)
	 * @param EcefX The X ECEF location
	 * @param EcefY The Y ECEF location
	 * @param EcefZ The Z ECEF location
	 * @param OutLatitudeDegrees The converted latitude in degrees
	 * @param OutLongitudeDegrees The converted longitude in degrees
	 * @param OutHeightMeters The converted height in meters
	 */
	static void CalculateLatLonHeightFromEcefXYZ(const double EcefX, const double EcefY, const double EcefZ, double& OutLatitudeDegrees, double& OutLongitudeDegrees, double& OutHeightMeters);

	/**
	 * Converts DIS X, Y, Z coordinates (ECEF) to Latitude, Longitude, and Height (LLH)
	 * @param EcefX The X ECEF location
	 * @param EcefY The Y ECEF location
	 * @param EcefZ The Z ECEF location
	 * @param OutLatitudeDegrees The converted latitude in degrees
	 * @param OutLongitudeDegrees The converted longitude in degrees
	 * @param OutHeightMeters The converted height in meters
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateLatLonHeightFromEcefXYZ(const float EcefX, const float EcefY, const float EcefZ, float& OutLatitudeDegrees, float& OutLongitudeDegrees, float& OutHeightMeters);

	/**
	 * Converts Latitude, Longitude, and Height (LLH) to DIS X, Y, Z coordinates (ECEF)
	 * @param LatitudeDegrees The latitude in degrees
	 * @param LongitudeDegrees The longitude in degrees
	 * @param HeightMeters The height in meters
	 * @param OutEcefX The converted X ECEF location
	 * @param OutEcefY The converted Y ECEF location
	 * @param OutEcefZ The converted Z ECEF location
	 */
	static void CalculateEcefXYZFromLatLonHeight(const double LatitudeDegrees, const double LongitudeDegrees, const double HeightMeters, double& OutEcefX, double& OutEcefY, double& OutEcefZ);

	/**
	 * Converts Latitude, Longitude, and Height (LLH) to DIS X, Y, Z coordinates (ECEF)
	 * @param LatitudeDegrees The latitude in degrees
	 * @param LongitudeDegrees The longitude in degrees
	 * @param HeightMeters The height in meters
	 * @param OutEcefX The converted X ECEF location
	 * @param OutEcefY The converted Y ECEF location
	 * @param OutEcefZ The converted Z ECEF location
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateEcefXYZFromLatLonHeight(const float LatitudeDegrees, const float LongitudeDegrees, const float HeightMeters, float& OutEcefX, float& OutEcefY, float& OutEcefZ);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta radians
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaRadians The desired amount to rotation in radians
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
		static void RotateVectorAroundAxisByRadians(glm::dvec3 VectorToRotate, double ThetaRadians, glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta radians
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaRadians The desired amount to rotation in radians
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void RotateVectorAroundAxisByRadians(FVector VectorToRotate, float ThetaRadians, FVector AxisVector, FVector& OutRotatedVector);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta degrees
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaDegrees The desired amount to rotation in degrees
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
		static void RotateVectorAroundAxisByDegrees(glm::dvec3 VectorToRotate, float ThetaDegrees, glm::dvec3 AxisVector, glm::dvec3& OutRotatedVector);

	/**
	 * Rotates the vector VectorToRotate around given axis AxisVector by Theta degrees
	 * @param VectorToRotate The target vector to rotate
	 * @param ThetaDegrees The desired amount to rotation in degrees
	 * @param AxisVector The vector indicating the axis of rotation
	 * @param OutRotatedVector The resultant rotated vector
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void RotateVectorAroundAxisByDegrees(FVector VectorToRotate, float ThetaDegrees, FVector AxisVector, FVector& OutRotatedVector);

	/**
	 * Applies the given heading, pitch, and roll in degrees to the local East North Down vectors
	 * @param HeadingDegrees The degrees from North of the facing direction (spin left and right)
	 * @param PitchDegrees The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees The degrees rotated about the local X axis (tilt left and right)
	 * @param NorthVector The vector pointing to the East
	 * @param EastVector The vector pointing toward the North
	 * @param DownVector The vector pointing away from the center of the Earth
	 * @param OutX The x axis (forward) vector with the heading and pitch applied
	 * @param OutY The y axis (right) vector with the heading and pitch applied
	 * @param OutZ the z axis (up) vector with the heading and pitch applied
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void ApplyHeadingPitchRollToNorthEastDownVector(const float HeadingDegrees, const float PitchDegrees, const float RollDegrees, const FVector NorthVector, const FVector EastVector, const FVector DownVector, FVector& OutX, FVector& OutY, FVector& OutZ);

	/**
	 * Calculates the East, North, and Up vectors at given latitude and longitude.
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param NorthVector The local vector pointing east at the given latitude and longitude
	 * @param EastVector The local vector pointing north at the given latitude and longitude
	 * @param DownVector The local vector pointing away from the center of the Earth at the given latitude and longitude
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateNorthEastDownVectorsFromLatLon(const float LatitudeDegrees, const float LongitudeDegrees, FVector& NorthVector, FVector& EastVector, FVector& DownVector);

	/**
	 * Calculates the East, North, and Up vectors at given latitude and longitude.
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param NorthVector The local vector pointing east at the given latitude and longitude
	 * @param EastVector The local vector pointing north at the given latitude and longitude
	 * @param DownVector The local vector pointing away from the center of the Earth at the given latitude and longitude
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateLatLongFromNorthEastDownVectors(FVector NorthVector, FVector EastVector, FVector DownVector, float& LatitudeDegrees, float& LongitudeDegrees);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in degrees with the given Heading, Pitch, and Roll in degrees at the given Latitude and Longitude.
	 * @param HeadingDegrees The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees The degrees rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiDegrees The rotation about the Z axis in degrees
	 * @param ThetaDegrees The rotation about the Y axis in degrees
	 * @param PhiDegrees The rotation about the X axis in degrees
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(const float HeadingDegrees, const float PitchDegrees, const float RollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiDegrees, float& ThetaDegrees, float& PhiDegrees);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in radians with the given Heading, Pitch, and Roll in radians at the given Latitude and Longitude.
	 * @param HeadingRadians The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians The radians rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiRadians The rotation about the Z axis in radians
	 * @param ThetaRadians The rotation about the Y axis in radians
	 * @param PhiRadians The rotation about the X axis in radians
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(const float HeadingRadians, const float PitchRadians, const float RollRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiRadians, float& ThetaRadians, float& PhiRadians);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in radians with the given Heading, Pitch, and Roll in degrees at the given Latitude and Longitude.
	 * @param HeadingDegrees The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees The degrees rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiRadians The rotation about the Z axis in radians
	 * @param ThetaRadians The rotation about the Y axis in radians
	 * @param PhiRadians The rotation about the X axis in radians
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(const float HeadingDegrees, const float PitchDegrees, const float RollDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiRadians, float& ThetaRadians, float& PhiRadians);

	/**
	 * Calculates the DIS orientation values Psi, Theta, and Phi in degrees with the given Heading, Pitch, and Roll in radians at the given Latitude and Longitude.
	 * @param HeadingRadians The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians The radians rotated about the local X axis (tilt left and right)
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param PsiDegrees The rotation about the Z axis in degrees
	 * @param ThetaDegrees The rotation about the Y axis in degrees
	 * @param PhiDegrees The rotation about the X axis in degrees
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(const float HeadingRadians, const float PitchRadians, const float RollRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& PsiDegrees, float& ThetaDegrees, float& PhiDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in degrees from the given DIS orientation values Psi, Theta, and Phi in degrees at the given Latitude and Longitude.
	 * @param PsiDegrees The rotation about the Z axis in degrees
	 * @param ThetaDegrees The rotation about the Y axis in degrees
	 * @param PhiDegrees The rotation about the X axis in degrees
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingDegrees The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees The degrees rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(const float PsiDegrees, const float ThetaDegrees, const float PhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingDegrees, float& PitchDegrees, float& RollDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in radians from the given DIS orientation values Psi, Theta, and Phi in radians at the given Latitude and Longitude.
	 * @param PsiRadians The rotation about the Z axis in radians
	 * @param ThetaRadians The rotation about the Y axis in radians
	 * @param PhiRadians The rotation about the X axis in radians
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingRadians The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians The radians rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(const float PsiRadians, const float ThetaRadians, const float PhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingRadians, float& PitchRadians, float& RollRadians);

	/**
	 * Calculates the Heading, Pitch, and Roll in degrees from the given DIS orientation values Psi, Theta, and Phi in radians at the given Latitude and Longitude.
	 * @param PsiRadians The rotation about the Z axis in radians
	 * @param ThetaRadians The rotation about the Y axis in radians
	 * @param PhiRadians The rotation about the X axis in radians
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingDegrees The degrees (-180 to 180) from North of the facing direction (spin left and right)
	 * @param PitchDegrees The degrees rotated about the local Y axis (front tip up and down)
	 * @param RollDegrees The degrees rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(const float PsiRadians, const float ThetaRadians, const float PhiRadians, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingDegrees, float& PitchDegrees, float& RollDegrees);

	/**
	 * Calculates the Heading, Pitch, and Roll in radians from the given DIS orientation values Psi, Theta, and Phi in degrees at the given Latitude and Longitude.
	 * @param PsiDegrees The rotation about the Z axis in degrees
	 * @param ThetaDegrees The rotation about the Y axis in degrees
	 * @param PhiDegrees The rotation about the X axis in degrees
	 * @param LatitudeDegrees The target latitude given in degrees
	 * @param LongitudeDegrees The target longitude given in degrees
	 * @param HeadingRadians The radians (-pi to pi) from North of the facing direction (spin left and right)
	 * @param PitchRadians The radians rotated about the local Y axis (front tip up and down)
	 * @param RollRadians The radians rotated about the local X axis (tilt left and right)
	 */
	UFUNCTION(BlueprintCallable, Category = "OpenDIS|Unit Conversions")
		static void CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(const float PsiDegrees, const float ThetaDegrees, const float PhiDegrees, const float LatitudeDegrees, const float LongitudeDegrees, float& HeadingRadians, float& PitchRadians, float& RollRadians);
};
