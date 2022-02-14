// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/WarfareFamilyPdu.h>
#include "GRILL_PDU.h"
#include "GRILL_WarfareFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FWarfareFamilyPdu : public FPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID FiringEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID TargetEntityID;
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_WarfareFamilyPDU : public UGRILL_PDU
{
	GENERATED_BODY()

public:
	UGRILL_WarfareFamilyPDU();

	void SetupFromOpenDIS(DIS::WarfareFamilyPdu* WarfareFamilyPDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FWarfareFamilyPdu WarfareFamilyPduStruct;

	DIS::WarfareFamilyPdu ToOpenDIS();

	virtual TArray<uint8> ToBytes() override;
};
