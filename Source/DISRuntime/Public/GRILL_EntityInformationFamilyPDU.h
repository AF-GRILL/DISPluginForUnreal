// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/EntityInformationFamilyPdu.h>
#include "GRILL_PDU.h"
#include "GRILL_EntityInformationFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FEntityInformationFamilyPdu : public FPDU
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_EntityInformationFamilyPDU : public UGRILL_PDU
{
	GENERATED_BODY()
	
public:
	UGRILL_EntityInformationFamilyPDU();
	
	void SetupFromOpenDIS(DIS::EntityInformationFamilyPdu* EntityInfoFamilyPDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityInformationFamilyPdu EntityInfoFamilyPduStruct;

	DIS::EntityInformationFamilyPdu ToOpenDIS();

	virtual TArray<uint8> ToBytes() override;
};
