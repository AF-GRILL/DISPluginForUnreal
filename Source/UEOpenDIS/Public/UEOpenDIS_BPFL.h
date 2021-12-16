// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UEOpenDISGameState.h"
#include "UEOpenDIS_BPFL.generated.h"

/**
 * 
 */
UCLASS()
class UEOPENDIS_API UUEOpenDIS_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static float GetHeadingFromEuler(float Lat, float Lon, float Psi, float Theta);
	static float GetPitchFromEuler(float Lat, float Lon, float Psi, float Theta);
	static float GetRollFromEuler(float Lat, float Lon, float Psi, float Theta, float Phi);
	static double GetHeadingFromEulerDouble(double Lat, double Lon, float Psi, float Theta);
	static double GetPitchFromEulerDouble(double Lat, double Lon, float Psi, float Theta);
	static double GetRollFromEulerDouble(double Lat, double Lon, float Psi, float Theta, float Phi);

	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void EulerToENU(float LatInRad, float LonInRad, float Psi, float Theta, float Phi, FRotator& TaitBryanAnglesOut);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void ECEF2UE4LocationESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LocationOut);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void ECEF2LongLatHeightESPDU(FEntityStatePDU EntityStatePDUIn, FVector& LonLatHeight);
	UFUNCTION(BlueprintCallable, Category = "OpenDIS | Unit Conversions")
		static void ECEF2ENU2UERotESPDU(FEntityStatePDU EntityStatePDUIn, FRotator& RotationOut);
};
