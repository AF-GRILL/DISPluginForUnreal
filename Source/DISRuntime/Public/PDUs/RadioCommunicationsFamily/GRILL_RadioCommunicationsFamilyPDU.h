// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/RadioCommunicationsFamilyPdu.h>
#include "PDUs/GRILL_PDU.h"
#include "GRILL_RadioCommunicationsFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FRadioCommunicationsFamilyPDU : public FPDU
{
	GENERATED_BODY()

	/** The site, application, and unique identifier for the entity that is the source of the communication. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|RadioCommunicationsFamily")
		FEntityID EntityID;
	/** The particular radio within an entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|RadioCommunicationsFamily")
		int32 RadioID;

	FRadioCommunicationsFamilyPDU() : FPDU()
	{

	}

	virtual ~FRadioCommunicationsFamilyPDU() {}

	void SetupFromOpenDIS(const DIS::RadioCommunicationsFamilyPdu& RadioCommunicationsFamilyPDUIn)
	{
		FPDU::SetupFromOpenDIS(RadioCommunicationsFamilyPDUIn);

		EntityID = RadioCommunicationsFamilyPDUIn.getEntityId();
		RadioID = RadioCommunicationsFamilyPDUIn.getRadioId();
	}

	void ToOpenDIS(DIS::RadioCommunicationsFamilyPdu& RadioCommunicationsFamilyPDUOut)
	{
		FPDU::ToOpenDIS(RadioCommunicationsFamilyPDUOut);

		RadioCommunicationsFamilyPDUOut.setEntityId(EntityID.ToOpenDIS());
		RadioCommunicationsFamilyPDUOut.setRadioId(RadioID);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);
		
		//marshal
		DIS::RadioCommunicationsFamilyPdu radioCommunicationsFamilyPDU;
		
		ToOpenDIS(radioCommunicationsFamilyPDU);
		radioCommunicationsFamilyPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};