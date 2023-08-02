// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/DistributedEmissionsFamilyPdu.h>
#include "PDUs/GRILL_PDU.h"
#include "GRILL_DistributedEmissionsFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FDistributedEmissionsFamilyPDU : public FPDU
{
	GENERATED_BODY()

	FDistributedEmissionsFamilyPDU() : FPDU()
	{
	}

	FDistributedEmissionsFamilyPDU(EPDUType Type) : FPDU()
	{
		PduType = Type;
	}

	virtual ~FDistributedEmissionsFamilyPDU() {}

	void SetupFromOpenDIS(const DIS::DistributedEmissionsFamilyPdu& DistributedEmissionsFamilyPDUIn)
	{
		FPDU::SetupFromOpenDIS(DistributedEmissionsFamilyPDUIn);
	}

	void ToOpenDIS(DIS::DistributedEmissionsFamilyPdu& DistributedEmissionsFamilyPDUOut)
	{
		FPDU::ToOpenDIS(DistributedEmissionsFamilyPDUOut);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::DistributedEmissionsFamilyPdu DistributedEmissionsFamilyPDU;

		ToOpenDIS(DistributedEmissionsFamilyPDU);
		DistributedEmissionsFamilyPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};