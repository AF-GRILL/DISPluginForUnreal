// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/SignalPdu.h>
#include "PDUs/RadioCommunicationsFamily/GRILL_RadioCommunicationsFamilyPDU.h"
#include "GRILL_SignalPDU.generated.h"

USTRUCT(BlueprintType)
struct FSignalPDU : public FRadioCommunicationsFamilyPDU
{
	GENERATED_BODY()

	/** This field shall specify the encoding used in the Data field of this PDU. The Encoding Scheme shall be composed of a 2-bit field specifying the encoding class and a 14-bit field specifying either the encoding type, or the number of TDL messages contained in this Signal PDU. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|RadioCommunicationsFamily|Signal")
		EEncodingType EncodingScheme;
	/** This field shall specify the TDL Type as a 16-bit enumeration field when the encoding class is the raw binary, audio, application - specific, or database index representation of a TDL Message. When the data field is not representing a TDL Message, this field shall be set to zero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|RadioCommunicationsFamily|Signal")
		ETDLType TDLType;
	/** This field shall specify either (1) the sample rate in samples per second if the encoding class is encoded audio or (2) the data rate in bits per second for data transmissions. If the Encoding Class is database index, this field shall be zero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|RadioCommunicationsFamily|Signal")
		int64 SampleRate;
	/** This field shall specify the number of samples in this PDU. If the Encoding Class is not encoded audio, this field shall be zero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|RadioCommunicationsFamily|Signal")
		int32 Samples;
	/** This field shall specify the audio or digital data conveyed by the radio transmission. The interpretation of the Data field depends on the value of the Encoding Scheme and TDL Type fields. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|RadioCommunicationsFamily|Signal")
		TArray<FOneByteChunk> Data;

	FSignalPDU() : FRadioCommunicationsFamilyPDU()
	{
		EncodingScheme = EEncodingType::Other;
		TDLType = ETDLType::Other;
		SampleRate = 0;
		Samples = 0;
	}

	virtual ~FSignalPDU() {}

	void SetupFromOpenDIS(const DIS::SignalPdu& SignalPDUIn)
	{
		FRadioCommunicationsFamilyPDU::SetupFromOpenDIS(SignalPDUIn);

		EncodingScheme = static_cast<EEncodingType>(SignalPDUIn.getEncodingScheme());
		TDLType = static_cast<ETDLType>(SignalPDUIn.getTdlType());
		SampleRate = SignalPDUIn.getSampleRate();
		Samples = SignalPDUIn.getSamples();

		Data.Empty();
		for (const auto& DataParamIn : SignalPDUIn.getData())
		{
			FOneByteChunk newDataParam;
			newDataParam.OtherParameters = TArray<uint8>(reinterpret_cast<const uint8*>(DataParamIn.getOtherParameters()), 1);

			Data.Add(newDataParam);
		}
	}

	void ToOpenDIS(DIS::SignalPdu& SignalPDUOut)
	{
		FRadioCommunicationsFamilyPDU::ToOpenDIS(SignalPDUOut);

		SignalPDUOut.setEncodingScheme(static_cast<unsigned short>(EncodingScheme));
		SignalPDUOut.setTdlType(static_cast<unsigned short>(TDLType));
		SignalPDUOut.setSampleRate(SampleRate);
		SignalPDUOut.setSamples(Samples);

		std::vector<DIS::OneByteChunk> outData;
		for (FOneByteChunk param : Data)
		{
			outData.push_back(param.ToOpenDIS());
		}
		SignalPDUOut.setData(outData);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::SignalPdu signalPDU;

		ToOpenDIS(signalPDU);
		signalPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};