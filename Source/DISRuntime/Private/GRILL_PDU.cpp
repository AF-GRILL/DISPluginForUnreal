// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_PDU.h"

UGRILL_PDU::UGRILL_PDU()
{
	PduStruct.ProtocolVersion = 6;
	PduStruct.ExerciseID = 0;
	PduStruct.PduType = EPDUType::Other;
	PduStruct.ProtocolFamily = 0;
	PduStruct.Timestamp = 0;
	PduStruct.Length = 0;
	PduStruct.Padding = 0;
}

void UGRILL_PDU::SetupFromOpenDIS(DIS::Pdu* PDUIn)
{
	PduStruct.ProtocolVersion = PDUIn->getProtocolVersion();
	PduStruct.ExerciseID = PDUIn->getExerciseID();
	PduStruct.PduType = static_cast<EPDUType>(PDUIn->getPduType());
	PduStruct.ProtocolFamily = PDUIn->getProtocolFamily();
	PduStruct.Timestamp = PDUIn->getTimestamp();
	PduStruct.Length = PDUIn->getLength();
	PduStruct.Padding = PDUIn->getPadding();
}

DIS::Pdu UGRILL_PDU::ToOpenDIS()
{
	DIS::Pdu pduOut;

	pduOut.setProtocolVersion(PduStruct.ProtocolVersion);
	pduOut.setExerciseID(PduStruct.ExerciseID);
	pduOut.setPduType(static_cast<unsigned char>(PduStruct.PduType));
	pduOut.setTimestamp(PduStruct.Timestamp);
	pduOut.setLength(PduStruct.Length);
	pduOut.setPadding(PduStruct.Padding);

    return pduOut;
}

TArray<uint8> UGRILL_PDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	ToOpenDIS().marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
