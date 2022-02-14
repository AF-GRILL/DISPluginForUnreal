// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_SimulationManagementFamilyPDU.h"

UGRILL_SimulationManagementFamilyPDU::UGRILL_SimulationManagementFamilyPDU()
{

}

void UGRILL_SimulationManagementFamilyPDU::SetupFromOpenDIS(DIS::SimulationManagementFamilyPdu* SimFamilyPDUIn)
{
    SimManagementFamilyPduStruct.ProtocolVersion = SimFamilyPDUIn->getProtocolVersion();
    SimManagementFamilyPduStruct.ExerciseID = SimFamilyPDUIn->getExerciseID();
    SimManagementFamilyPduStruct.PduType = static_cast<EPDUType>(SimFamilyPDUIn->getPduType());
    SimManagementFamilyPduStruct.ProtocolFamily = SimFamilyPDUIn->getProtocolFamily();
    SimManagementFamilyPduStruct.Timestamp = SimFamilyPDUIn->getTimestamp();
    SimManagementFamilyPduStruct.Length = SimFamilyPDUIn->getLength();
    SimManagementFamilyPduStruct.Padding = SimFamilyPDUIn->getPadding();

    SimManagementFamilyPduStruct.OriginatingEntityID = SimFamilyPDUIn->getOriginatingEntityID();
    SimManagementFamilyPduStruct.ReceivingEntityID = SimFamilyPDUIn->getReceivingEntityID();
}

DIS::SimulationManagementFamilyPdu UGRILL_SimulationManagementFamilyPDU::ToOpenDIS()
{
    DIS::SimulationManagementFamilyPdu simFamilyPDUOut;

    simFamilyPDUOut.setProtocolVersion(SimManagementFamilyPduStruct.ProtocolVersion);
    simFamilyPDUOut.setExerciseID(SimManagementFamilyPduStruct.ExerciseID);
    simFamilyPDUOut.setPduType(static_cast<unsigned char>(SimManagementFamilyPduStruct.PduType));
    simFamilyPDUOut.setTimestamp(SimManagementFamilyPduStruct.Timestamp);
    simFamilyPDUOut.setLength(SimManagementFamilyPduStruct.Length);
    simFamilyPDUOut.setPadding(SimManagementFamilyPduStruct.Padding);

    simFamilyPDUOut.setOriginatingEntityID(SimManagementFamilyPduStruct.OriginatingEntityID.ToOpenDIS());
    simFamilyPDUOut.setOriginatingEntityID(SimManagementFamilyPduStruct.OriginatingEntityID.ToOpenDIS());

    return simFamilyPDUOut;
}

TArray<uint8> UGRILL_SimulationManagementFamilyPDU::ToBytes()
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
