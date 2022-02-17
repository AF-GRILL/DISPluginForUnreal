// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_EntityInformationFamilyPDU.h"

UGRILL_EntityInformationFamilyPDU::UGRILL_EntityInformationFamilyPDU() : UGRILL_PDU()
{

}

void UGRILL_EntityInformationFamilyPDU::SetupFromOpenDIS(DIS::EntityInformationFamilyPdu* EntityInfoFamilyPDUIn)
{
	UGRILL_PDU::SetupFromOpenDIS(EntityInfoFamilyPDUIn);
}

void UGRILL_EntityInformationFamilyPDU::ToOpenDIS(DIS::EntityInformationFamilyPdu& EntityInfoFamilyPDUOut)
{
	UGRILL_PDU::ToOpenDIS(EntityInfoFamilyPDUOut);
}

TArray<uint8> UGRILL_EntityInformationFamilyPDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::EntityInformationFamilyPdu entityInfoFamilyPDU;

	ToOpenDIS(entityInfoFamilyPDU);
	entityInfoFamilyPDU.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
