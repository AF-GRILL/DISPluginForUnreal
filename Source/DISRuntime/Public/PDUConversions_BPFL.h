// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PDUMasterInclude.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PDUConversions_BPFL.generated.h"

/**
 * 
 */
UCLASS()
class DISRUNTIME_API UPDUConversions_BPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertPDUToBytes(FPDU PDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertEntityInfoFamilyPDUToBytes(FEntityInformationFamilyPDU EntityInfoFamilyPDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertSimManagementFamilyPDUToBytes(FSimulationManagementFamilyPDU SimManagementFamilyPDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertWarfareFamilyPDUToBytes(FWarfareFamilyPDU WarfareFamilyPDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertEntityStatePDUToBytes(FEntityStatePDU EntityStatePDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertEntityStateUpdatePDUToBytes(FEntityStateUpdatePDU EntityStateUpdatePDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertFirePDUToBytes(FFirePDU FirePDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertDetonationPDUToBytes(FDetonationPDU DetonationPDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertRemoveEntityPDUToBytes(FRemoveEntityPDU RemoveEntityPDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertStartResumePDUToBytes(FStartResumePDU StartResumePDUIn);

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
		static TArray<uint8> ConvertStopFreezePDUToBytes(FStopFreezePDU StopFreezePDUIn);
};
