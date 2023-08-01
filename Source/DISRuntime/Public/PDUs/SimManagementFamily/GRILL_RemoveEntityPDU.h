// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/RemoveEntityPdu.h>
#include "PDUs/SimManagementFamily/GRILL_SimulationManagementFamilyPDU.h"
#include "GRILL_RemoveEntityPDU.generated.h"

USTRUCT(BlueprintType)
struct FRemoveEntityPDU : public FSimulationManagementFamilyPDU
{
	GENERATED_BODY()

	/** The specific and unique entity removal request ID. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MinClamp = "0", MaxClamp = "4294967295"), Category = "GRILL DIS|Structs|PDUs|RemoveEntity")
		int64 RequestID;

	FRemoveEntityPDU() : FSimulationManagementFamilyPDU()
	{
		PduType = EPDUType::RemoveEntity;

		RequestID = 0;
	}

	virtual ~FRemoveEntityPDU() {}

	void SetupFromOpenDIS(const DIS::RemoveEntityPdu& RemoveEntityPDUIn)
	{
		FSimulationManagementFamilyPDU::SetupFromOpenDIS(RemoveEntityPDUIn);

		RequestID = RemoveEntityPDUIn.getRequestID();
	}

	void ToOpenDIS(DIS::RemoveEntityPdu& RemoveEntityPDUOut)
	{
		FSimulationManagementFamilyPDU::ToOpenDIS(RemoveEntityPDUOut);

		// Remove entity specific PDU setup
		RemoveEntityPDUOut.setRequestID(RequestID);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::RemoveEntityPdu removeEntityPDU;

		ToOpenDIS(removeEntityPDU);
		removeEntityPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};