// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/Pdu.h>
#include "DISEnumsAndStructs.h"
#include "GRILL_PDU.generated.h"

USTRUCT(BlueprintType)
struct FPDU 
{
	GENERATED_BODY()

	/** The version of the protocol. 5=DIS-1995, 6=DIS-1998. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 ProtocolVersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 ExerciseID;

	/** Type of pdu, unique for each PDU struct */
	EPDUType PduType;

	/** Value that refers to the protocol family, e.g. SimulationManagement, etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 ProtocolFamily;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Timestamp;

	/** Length, in bytes, of the PDU */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 Length;

	/** Zero-filled array of padding */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Padding;

	FPDU()
	{
		ProtocolVersion = 6;
		ExerciseID = 0;
		PduType = EPDUType::Other;
		ProtocolFamily = 0;
		Timestamp = 0;
		Length = 0;
		Padding = 0;
	}

	virtual ~FPDU() {}

	void SetupFromOpenDIS(DIS::Pdu* PDUIn)
	{
		ProtocolVersion = PDUIn->getProtocolVersion();
		ExerciseID = PDUIn->getExerciseID();
		PduType = static_cast<EPDUType>(PDUIn->getPduType());
		ProtocolFamily = PDUIn->getProtocolFamily();
		Timestamp = PDUIn->getTimestamp();
		Length = PDUIn->getLength();
		Padding = PDUIn->getPadding();
	}

	void ToOpenDIS(DIS::Pdu& PDUOut)
	{
		PDUOut.setProtocolVersion(ProtocolVersion);
		PDUOut.setExerciseID(ExerciseID);
		PDUOut.setPduType(static_cast<unsigned char>(PduType));
		PDUOut.setTimestamp(Timestamp);
		PDUOut.setLength(Length);
		PDUOut.setPadding(Padding);
	}

	virtual TArray<uint8> ToBytes()
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::Pdu pdu;

		ToOpenDIS(pdu);
		pdu.marshal(buffer);

		return DISDataStreamToBytes(buffer);
	}

	TArray<uint8> DISDataStreamToBytes(DIS::DataStream DataStream) 
	{
		TArray<uint8> byteArrayOut;
		byteArrayOut.Init(0, DataStream.size());

		for (int i = 0; i < DataStream.size(); i++) {
			byteArrayOut[i] = DataStream[i];
		}

		return byteArrayOut;
	}
};