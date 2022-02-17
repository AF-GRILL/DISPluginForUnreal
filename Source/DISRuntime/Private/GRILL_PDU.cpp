// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_PDU.h"

UGRILL_PDU::UGRILL_PDU()
{

}

void UGRILL_PDU::SetupFromOpenDIS(DIS::Pdu* PDUIn)
{
	PDUStruct.ProtocolVersion = PDUIn->getProtocolVersion();
	PDUStruct.ExerciseID = PDUIn->getExerciseID();
	PDUStruct.PduType = static_cast<EPDUType>(PDUIn->getPduType());
	PDUStruct.ProtocolFamily = PDUIn->getProtocolFamily();
	PDUStruct.Timestamp = PDUIn->getTimestamp();
	PDUStruct.Length = PDUIn->getLength();
	PDUStruct.Padding = PDUIn->getPadding();
}

void UGRILL_PDU::ToOpenDIS(DIS::Pdu& PDUOut)
{
	PDUOut.setProtocolVersion(PDUStruct.ProtocolVersion);
	PDUOut.setExerciseID(PDUStruct.ExerciseID);
	PDUOut.setPduType(static_cast<unsigned char>(PDUStruct.PduType));
	PDUOut.setTimestamp(PDUStruct.Timestamp);
	PDUOut.setLength(PDUStruct.Length);
	PDUOut.setPadding(PDUStruct.Padding);
}

TArray<uint8> UGRILL_PDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::Pdu pdu;

	ToOpenDIS(pdu);
	pdu.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
