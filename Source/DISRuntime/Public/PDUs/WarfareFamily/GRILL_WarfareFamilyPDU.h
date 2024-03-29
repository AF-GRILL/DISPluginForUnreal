// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/WarfareFamilyPdu.h>
#include "PDUs/GRILL_PDU.h"
#include "GRILL_WarfareFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FWarfareFamilyPDU : public FPDU
{
	GENERATED_BODY()

	/** The ID of the firing entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|WarfareFamily")
		FEntityID FiringEntityID;
	/** The ID of the intended target if known */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|WarfareFamily")
		FEntityID TargetEntityID; 
	
	FWarfareFamilyPDU() : FPDU()
	{

	}

	virtual ~FWarfareFamilyPDU() {}

	void SetupFromOpenDIS(const DIS::WarfareFamilyPdu& WarfareFamilyPDUIn)
	{
		FPDU::SetupFromOpenDIS(WarfareFamilyPDUIn);

		FiringEntityID = WarfareFamilyPDUIn.getFiringEntityID();
		TargetEntityID = WarfareFamilyPDUIn.getTargetEntityID();
	}

	void ToOpenDIS(DIS::WarfareFamilyPdu& WarfareFamilyPDUOut)
	{
		FPDU::ToOpenDIS(WarfareFamilyPDUOut);

		WarfareFamilyPDUOut.setFiringEntityID(FiringEntityID.ToOpenDIS());
		WarfareFamilyPDUOut.setTargetEntityID(TargetEntityID.ToOpenDIS());
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::WarfareFamilyPdu warfareFamilyPDU;

		ToOpenDIS(warfareFamilyPDU);
		warfareFamilyPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};