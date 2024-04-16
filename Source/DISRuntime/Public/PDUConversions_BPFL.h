// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

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
	static TArray<uint8> ConvertPDUToBytes(FPDU PDUIn)
	{
		return PDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertEntityInfoFamilyPDUToBytes(FEntityInformationFamilyPDU EntityInfoFamilyPDUIn)
	{
		return EntityInfoFamilyPDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertSimManagementFamilyPDUToBytes(FSimulationManagementFamilyPDU SimManagementFamilyPDUIn)
	{
		return SimManagementFamilyPDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertWarfareFamilyPDUToBytes(FWarfareFamilyPDU WarfareFamilyPDUIn)
	{
		return WarfareFamilyPDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertRadioCommunicationsFamilyPDUToBytes(FRadioCommunicationsFamilyPDU RadioCommunicationsFamilyPDUIn)
	{
		return RadioCommunicationsFamilyPDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertEntityStatePDUToBytes(FEntityStatePDU EntityStatePDUIn)
	{
		return EntityStatePDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertEntityStateUpdatePDUToBytes(FEntityStateUpdatePDU EntityStateUpdatePDUIn)
	{
		return EntityStateUpdatePDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertFirePDUToBytes(FFirePDU FirePDUIn)
	{
		return FirePDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertDetonationPDUToBytes(FDetonationPDU DetonationPDUIn)
	{
		return DetonationPDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertRemoveEntityPDUToBytes(FRemoveEntityPDU RemoveEntityPDUIn)
	{
		return RemoveEntityPDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertStartResumePDUToBytes(FStartResumePDU StartResumePDUIn)
	{
		return StartResumePDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertStopFreezePDUToBytes(FStopFreezePDU StopFreezePDUIn)
	{
		return StopFreezePDUIn.ToBytes();
	}

	UFUNCTION(BlueprintPure, Category = "GRILL DIS|PDU Conversions")
	static TArray<uint8> ConvertSignalPDUToBytes(FSignalPDU SignalPDUIn)
	{
		return SignalPDUIn.ToBytes();
	}
};
