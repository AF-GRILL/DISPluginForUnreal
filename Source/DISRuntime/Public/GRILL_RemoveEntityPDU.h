// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/RemoveEntityPdu.h>
#include "GRILL_SimulationManagementFamilyPDU.h"
#include "GRILL_RemoveEntityPDU.generated.h"

USTRUCT(BlueprintType)
struct FRemoveEntityPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FRemoveEntityPDU()
	{
		RequestID = 0;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_RemoveEntityPDU : public UGRILL_SimulationManagementFamilyPDU
{
	GENERATED_BODY()
	
public:
	UGRILL_RemoveEntityPDU();

	void SetupFromOpenDIS(DIS::RemoveEntityPdu* RemoveEntityPDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRemoveEntityPDU RemoveEntityPDUStruct;

	void ToOpenDIS(DIS::RemoveEntityPdu& RemoveEntityPDUOut);

	virtual TArray<uint8> ToBytes() override;	
};
