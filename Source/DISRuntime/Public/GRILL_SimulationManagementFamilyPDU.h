// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/SimulationManagementFamilyPdu.h>
#include "GRILL_PDU.h"
#include "GRILL_SimulationManagementFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FSimulationManagementFamilyPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID OriginatingEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID ReceivingEntityID;
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_SimulationManagementFamilyPDU : public UGRILL_PDU
{
	GENERATED_BODY()
	
public:
	UGRILL_SimulationManagementFamilyPDU();

	void SetupFromOpenDIS(DIS::SimulationManagementFamilyPdu* SimFamilyPDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSimulationManagementFamilyPDU SimManagementFamilyPDUStruct;

	void ToOpenDIS(DIS::SimulationManagementFamilyPdu& SimFamilyPDUOut);

	virtual TArray<uint8> ToBytes() override;		
};
