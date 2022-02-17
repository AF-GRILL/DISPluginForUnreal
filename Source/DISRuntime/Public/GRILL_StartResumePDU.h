// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/StartResumePdu.h>
#include "GRILL_SimulationManagementFamilyPDU.h"
#include "GRILL_StartResumePDU.generated.h"

USTRUCT(BlueprintType)
struct FStartResumePDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime RealWorldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime SimulationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FStartResumePDU()
	{
		RequestID = 0;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_StartResumePDU : public UGRILL_SimulationManagementFamilyPDU
{
	GENERATED_BODY()
	
public:
	UGRILL_StartResumePDU();

	void SetupFromOpenDIS(DIS::StartResumePdu* StartResumePDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FStartResumePDU StartResumePDUStruct;

	void ToOpenDIS(DIS::StartResumePdu& StartResumeFamilyPDUOut);

	virtual TArray<uint8> ToBytes() override;	
};
