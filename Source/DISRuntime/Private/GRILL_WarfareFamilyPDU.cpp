// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_WarfareFamilyPDU.h"

UGRILL_WarfareFamilyPDU::UGRILL_WarfareFamilyPDU() : UGRILL_PDU()
{

}

void UGRILL_WarfareFamilyPDU::SetupFromOpenDIS(DIS::WarfareFamilyPdu* WarfareFamilyPDUIn)
{
	UGRILL_PDU::SetupFromOpenDIS(WarfareFamilyPDUIn);

	WarfareFamilyPDUStruct.FiringEntityID = WarfareFamilyPDUIn->getFiringEntityID();
	WarfareFamilyPDUStruct.TargetEntityID = WarfareFamilyPDUIn->getTargetEntityID();
}

void UGRILL_WarfareFamilyPDU::ToOpenDIS(DIS::WarfareFamilyPdu& WarfareFamilyPDUOut)
{
	UGRILL_PDU::ToOpenDIS(WarfareFamilyPDUOut);

	WarfareFamilyPDUOut.setFiringEntityID(WarfareFamilyPDUStruct.FiringEntityID.ToOpenDIS());
	WarfareFamilyPDUOut.setTargetEntityID(WarfareFamilyPDUStruct.TargetEntityID.ToOpenDIS());
}

TArray<uint8> UGRILL_WarfareFamilyPDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::WarfareFamilyPdu warfareFamilyPDU;

	ToOpenDIS(warfareFamilyPDU);
	warfareFamilyPDU.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
