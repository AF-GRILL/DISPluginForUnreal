// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/StopFreezePdu.h>
#include "GRILL_SimulationManagementFamilyPDU.h"
#include "GRILL_StopFreezePDU.generated.h"

USTRUCT(BlueprintType)
struct FStopFreezePDU : public FSimulationManagementFamilyPdu
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime RealWorldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EReason Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FrozenBehavior;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PaddingOne;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FStopFreezePDU()
	{
		PduType = EPDUType::Stop_Freeze;
		Reason = EReason::Other;
		FrozenBehavior = 0;
		PaddingOne = 0;
		RequestID = 0;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_StopFreezePDU : public UGRILL_SimulationManagementFamilyPDU
{
	GENERATED_BODY()
	
public:
	UGRILL_StopFreezePDU();

	void SetupFromOpenDIS(DIS::StopFreezePdu* StopFreezePDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStopFreezePDU StopFreezePduStruct;
	
	DIS::StopFreezePdu ToOpenDIS();
	
	virtual TArray<uint8> ToBytes() override;
};
