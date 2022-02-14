// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_RemoveEntityPDU.h"

UGRILL_RemoveEntityPDU::UGRILL_RemoveEntityPDU()
{
	RemoveEntityPduStruct.PduType = EPDUType::RemoveEntity;
	RemoveEntityPduStruct.RequestID = 0;
}

void UGRILL_RemoveEntityPDU::SetupFromOpenDIS(DIS::RemoveEntityPdu* RemoveEntityPDUIn)
{
	//pdu common parameters
	RemoveEntityPduStruct.ProtocolVersion = RemoveEntityPDUIn->getProtocolVersion();
	RemoveEntityPduStruct.ExerciseID = RemoveEntityPDUIn->getExerciseID();
	RemoveEntityPduStruct.PduType = static_cast<EPDUType>(RemoveEntityPDUIn->getPduType());
	RemoveEntityPduStruct.ProtocolFamily = RemoveEntityPDUIn->getProtocolFamily();
	RemoveEntityPduStruct.Timestamp = RemoveEntityPDUIn->getTimestamp();
	RemoveEntityPduStruct.Length = RemoveEntityPDUIn->getLength();
	RemoveEntityPduStruct.Padding = RemoveEntityPDUIn->getPadding();

	//Simulation Management Family Pdu specific
	RemoveEntityPduStruct.OriginatingEntityID = RemoveEntityPDUIn->getOriginatingEntityID();
	RemoveEntityPduStruct.ReceivingEntityID = RemoveEntityPDUIn->getReceivingEntityID();

	RemoveEntityPduStruct.RequestID = RemoveEntityPDUIn->getRequestID();
}

DIS::RemoveEntityPdu UGRILL_RemoveEntityPDU::ToOpenDIS()
{
	DIS::RemoveEntityPdu removeEntityPDUOut;

	// Common PDU setup
	removeEntityPDUOut.setProtocolVersion(RemoveEntityPduStruct.ProtocolVersion);
	removeEntityPDUOut.setExerciseID(RemoveEntityPduStruct.ExerciseID);
	removeEntityPDUOut.setPduType(static_cast<unsigned char>(RemoveEntityPduStruct.PduType));
	removeEntityPDUOut.setProtocolFamily(RemoveEntityPduStruct.ProtocolFamily);
	removeEntityPDUOut.setTimestamp(RemoveEntityPduStruct.Timestamp);
	removeEntityPDUOut.setLength(RemoveEntityPduStruct.Length);
	removeEntityPDUOut.setPadding(RemoveEntityPduStruct.Padding);

	// Simulation Management Family PDU setup
	removeEntityPDUOut.setOriginatingEntityID(RemoveEntityPduStruct.OriginatingEntityID.ToOpenDIS());
	removeEntityPDUOut.setReceivingEntityID(RemoveEntityPduStruct.ReceivingEntityID.ToOpenDIS());

	// Remove entity specific PDU setup
	removeEntityPDUOut.setOriginatingEntityID(RemoveEntityPduStruct.OriginatingEntityID.ToOpenDIS());
	removeEntityPDUOut.setReceivingEntityID(RemoveEntityPduStruct.ReceivingEntityID.ToOpenDIS());
	removeEntityPDUOut.setRequestID(RemoveEntityPduStruct.RequestID);

	return removeEntityPDUOut;
}

TArray<uint8> UGRILL_RemoveEntityPDU::ToBytes()
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
