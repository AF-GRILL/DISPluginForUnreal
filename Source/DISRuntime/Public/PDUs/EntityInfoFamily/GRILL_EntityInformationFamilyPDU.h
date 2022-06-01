// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/EntityInformationFamilyPdu.h>
#include "PDUs/GRILL_PDU.h"
#include "GRILL_EntityInformationFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FEntityInformationFamilyPDU : public FPDU
{
	GENERATED_BODY()

	FEntityInformationFamilyPDU() : FPDU()
	{

	}

	virtual ~FEntityInformationFamilyPDU() {}

	void SetupFromOpenDIS(DIS::EntityInformationFamilyPdu* EntityInfoFamilyPDUIn)
	{
		FPDU::SetupFromOpenDIS(EntityInfoFamilyPDUIn);
	}

	void ToOpenDIS(DIS::EntityInformationFamilyPdu& EntityInfoFamilyPDUOut)
	{
		FPDU::ToOpenDIS(EntityInfoFamilyPDUOut);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::EntityInformationFamilyPdu entityInfoFamilyPDU;

		ToOpenDIS(entityInfoFamilyPDU);
		entityInfoFamilyPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};