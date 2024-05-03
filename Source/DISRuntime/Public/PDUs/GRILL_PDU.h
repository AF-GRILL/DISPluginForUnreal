// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/Pdu.h>
#include "EnumsAndStructs/DISEnumsAndStructs.h"
#include "GRILL_PDU.generated.h"

USTRUCT(BlueprintType)
struct FPDU 
{
	GENERATED_BODY()

	/** The version of the protocol. 5=DIS-1995, 6=DIS-1998. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs")
		EProtocolVersion ProtocolVersion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs")
		uint8 ExerciseID;

	/** Type of pdu, unique for each PDU struct */
	EPDUType PduType;

	/** Value that refers to the protocol family, e.g. SimulationManagement, etc. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs")
		EProtocolFamily ProtocolFamily;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "4294967295"), Category = "GRILL DIS|Structs|PDUs")
		FTimestamp Timestamp;

	/** Length, in bytes, of the PDU */
	UPROPERTY(meta = (ClampMin = "0", ClampMax = "65535"))
		int32 Length;

	/** Zero-filled array of padding */
	UPROPERTY()
		int32 Padding;

	FPDU()
	{
		ProtocolVersion = EProtocolVersion::IEEE1278_1A_1998;
		ExerciseID = 0;
		PduType = EPDUType::Other;
		ProtocolFamily = EProtocolFamily::Other;
		Length = 0;
		Padding = 0;
	}

	virtual ~FPDU() {}

	void SetupFromOpenDIS(const DIS::Pdu& PDUIn)
	{
		ProtocolVersion = static_cast<EProtocolVersion>(PDUIn.getProtocolVersion());
		ExerciseID = PDUIn.getExerciseID();
		PduType = static_cast<EPDUType>(PDUIn.getPduType());
		ProtocolFamily = static_cast<EProtocolFamily>(PDUIn.getProtocolFamily());
		Timestamp = PDUIn.getTimestamp();
		Length = PDUIn.getLength();
		Padding = PDUIn.getPadding();
	}

	void ToOpenDIS(DIS::Pdu& PDUOut)
	{
		PDUOut.setProtocolVersion(static_cast<unsigned char>(ProtocolVersion));
		PDUOut.setExerciseID(ExerciseID);
		PDUOut.setPduType(static_cast<unsigned char>(PduType));
		PDUOut.setProtocolFamily(static_cast<unsigned char>(ProtocolFamily));
		PDUOut.setTimestamp(Timestamp.ToOpenDIS());
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

	TArray<uint8> DISDataStreamToBytes(const DIS::DataStream& DataStream) 
	{
		TArray<uint8> byteArrayOut;
		byteArrayOut.Init(0, DataStream.size());

		for (int i = 0; i < DataStream.size(); i++) {
			byteArrayOut[i] = DataStream[i];
		}

		return byteArrayOut;
	}
};