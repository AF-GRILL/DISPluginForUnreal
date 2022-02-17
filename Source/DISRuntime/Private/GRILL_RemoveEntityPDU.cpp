// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_RemoveEntityPDU.h"

UGRILL_RemoveEntityPDU::UGRILL_RemoveEntityPDU() : UGRILL_SimulationManagementFamilyPDU()
{
	PDUStruct.PduType = EPDUType::RemoveEntity;
}

void UGRILL_RemoveEntityPDU::SetupFromOpenDIS(DIS::RemoveEntityPdu* RemoveEntityPDUIn)
{
	UGRILL_SimulationManagementFamilyPDU::SetupFromOpenDIS(RemoveEntityPDUIn);

	RemoveEntityPDUStruct.RequestID = RemoveEntityPDUIn->getRequestID();
}

void UGRILL_RemoveEntityPDU::ToOpenDIS(DIS::RemoveEntityPdu& RemoveEntityPDUOut)
{
	UGRILL_SimulationManagementFamilyPDU::ToOpenDIS(RemoveEntityPDUOut);

	// Remove entity specific PDU setup
	RemoveEntityPDUOut.setRequestID(RemoveEntityPDUStruct.RequestID);
}

TArray<uint8> UGRILL_RemoveEntityPDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::RemoveEntityPdu removeEntityPDU;

	ToOpenDIS(removeEntityPDU);
	removeEntityPDU.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
