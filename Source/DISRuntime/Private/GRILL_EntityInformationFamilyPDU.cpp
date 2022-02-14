// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_EntityInformationFamilyPDU.h"

UGRILL_EntityInformationFamilyPDU::UGRILL_EntityInformationFamilyPDU()
{

}

void UGRILL_EntityInformationFamilyPDU::SetupFromOpenDIS(DIS::EntityInformationFamilyPdu* EntityInfoFamilyPDUIn)
{
	EntityInfoFamilyPduStruct.ProtocolVersion = EntityInfoFamilyPDUIn->getProtocolVersion();
	EntityInfoFamilyPduStruct.ExerciseID = EntityInfoFamilyPDUIn->getExerciseID();
	EntityInfoFamilyPduStruct.PduType = static_cast<EPDUType>(EntityInfoFamilyPDUIn->getPduType());
	EntityInfoFamilyPduStruct.ProtocolFamily = EntityInfoFamilyPDUIn->getProtocolFamily();
	EntityInfoFamilyPduStruct.Timestamp = EntityInfoFamilyPDUIn->getTimestamp();
	EntityInfoFamilyPduStruct.Length = EntityInfoFamilyPDUIn->getLength();
	EntityInfoFamilyPduStruct.Padding = EntityInfoFamilyPDUIn->getPadding();
}

DIS::EntityInformationFamilyPdu UGRILL_EntityInformationFamilyPDU::ToOpenDIS()
{
	DIS::EntityInformationFamilyPdu entityInfoFamilyPDUOut;

	entityInfoFamilyPDUOut.setProtocolVersion(EntityInfoFamilyPduStruct.ProtocolVersion);
	entityInfoFamilyPDUOut.setExerciseID(EntityInfoFamilyPduStruct.ExerciseID);
	entityInfoFamilyPDUOut.setPduType(static_cast<unsigned char>(EntityInfoFamilyPduStruct.PduType));
	entityInfoFamilyPDUOut.setTimestamp(EntityInfoFamilyPduStruct.Timestamp);
	entityInfoFamilyPDUOut.setLength(EntityInfoFamilyPduStruct.Length);
	entityInfoFamilyPDUOut.setPadding(EntityInfoFamilyPduStruct.Padding);

	return entityInfoFamilyPDUOut;
}

TArray<uint8> UGRILL_EntityInformationFamilyPDU::ToBytes()
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
