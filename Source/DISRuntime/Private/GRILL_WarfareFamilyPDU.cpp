// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_WarfareFamilyPDU.h"

UGRILL_WarfareFamilyPDU::UGRILL_WarfareFamilyPDU()
{

}

void UGRILL_WarfareFamilyPDU::SetupFromOpenDIS(DIS::WarfareFamilyPdu* WarfareFamilyPDUIn)
{
	WarfareFamilyPduStruct.ProtocolVersion = WarfareFamilyPDUIn->getProtocolVersion();
	WarfareFamilyPduStruct.ExerciseID = WarfareFamilyPDUIn->getExerciseID();
	WarfareFamilyPduStruct.PduType = static_cast<EPDUType>(WarfareFamilyPDUIn->getPduType());
	WarfareFamilyPduStruct.ProtocolFamily = WarfareFamilyPDUIn->getProtocolFamily();
	WarfareFamilyPduStruct.Timestamp = WarfareFamilyPDUIn->getTimestamp();
	WarfareFamilyPduStruct.Length = WarfareFamilyPDUIn->getLength();
	WarfareFamilyPduStruct.Padding = WarfareFamilyPDUIn->getPadding();

	WarfareFamilyPduStruct.FiringEntityID = WarfareFamilyPDUIn->getFiringEntityID();
	WarfareFamilyPduStruct.TargetEntityID = WarfareFamilyPDUIn->getTargetEntityID();
}

DIS::WarfareFamilyPdu UGRILL_WarfareFamilyPDU::ToOpenDIS()
{
	DIS::WarfareFamilyPdu warfareFamilyPDUOut;

	warfareFamilyPDUOut.setProtocolVersion(WarfareFamilyPduStruct.ProtocolVersion);
	warfareFamilyPDUOut.setExerciseID(WarfareFamilyPduStruct.ExerciseID);
	warfareFamilyPDUOut.setPduType(static_cast<unsigned char>(WarfareFamilyPduStruct.PduType));
	warfareFamilyPDUOut.setTimestamp(WarfareFamilyPduStruct.Timestamp);
	warfareFamilyPDUOut.setLength(WarfareFamilyPduStruct.Length);
	warfareFamilyPDUOut.setPadding(WarfareFamilyPduStruct.Padding);

	warfareFamilyPDUOut.setFiringEntityID(WarfareFamilyPduStruct.FiringEntityID.ToOpenDIS());
	warfareFamilyPDUOut.setTargetEntityID(WarfareFamilyPduStruct.TargetEntityID.ToOpenDIS());

	return warfareFamilyPDUOut;
}

TArray<uint8> UGRILL_WarfareFamilyPDU::ToBytes()
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
