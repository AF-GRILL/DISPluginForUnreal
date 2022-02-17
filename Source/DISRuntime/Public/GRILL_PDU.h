// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/Pdu.h>
#include "DISEnumsAndStructs.h"
#include "GRILL_PDU.generated.h"

USTRUCT(BlueprintType)
struct FPDU 
{
	GENERATED_BODY()

	/** The version of the protocol. 5=DIS-1995, 6=DIS-1998. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 ProtocolVersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 ExerciseID;

	/** Type of pdu, unique for each PDU struct */
	EPDUType PduType;

	/** Value that refers to the protocol family, e.g. SimulationManagement, etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 ProtocolFamily;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Timestamp;

	/** Length, in bytes, of the PDU */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Length;

	/** Zero-filled array of padding */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Padding;

	FPDU()
	{
		ProtocolVersion = 6;
		ExerciseID = 0;
		PduType = EPDUType::Other;
		ProtocolFamily = 0;
		Timestamp = 0;
		Length = 0;
		Padding = 0;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_PDU : public UObject
{
	GENERATED_BODY()

public:
	UGRILL_PDU();

	void SetupFromOpenDIS(DIS::Pdu* PDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPDU PDUStruct;
	
	void ToOpenDIS(DIS::Pdu& PDUOut);

	UFUNCTION(BlueprintCallable, Category = "GRILL DIS|PDUs")
		virtual TArray<uint8> ToBytes();
};
