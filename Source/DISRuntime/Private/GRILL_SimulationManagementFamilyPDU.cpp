// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_SimulationManagementFamilyPDU.h"

UGRILL_SimulationManagementFamilyPDU::UGRILL_SimulationManagementFamilyPDU() : UGRILL_PDU()
{

}

void UGRILL_SimulationManagementFamilyPDU::SetupFromOpenDIS(DIS::SimulationManagementFamilyPdu* SimFamilyPDUIn)
{
    UGRILL_PDU::SetupFromOpenDIS(SimFamilyPDUIn);

    SimManagementFamilyPDUStruct.OriginatingEntityID = SimFamilyPDUIn->getOriginatingEntityID();
    SimManagementFamilyPDUStruct.ReceivingEntityID = SimFamilyPDUIn->getReceivingEntityID();
}

void UGRILL_SimulationManagementFamilyPDU::ToOpenDIS(DIS::SimulationManagementFamilyPdu& simFamilyPDUOut)
{
    UGRILL_PDU::ToOpenDIS(simFamilyPDUOut);

    simFamilyPDUOut.setOriginatingEntityID(SimManagementFamilyPDUStruct.OriginatingEntityID.ToOpenDIS());
    simFamilyPDUOut.setReceivingEntityID(SimManagementFamilyPDUStruct.ReceivingEntityID.ToOpenDIS());
}

TArray<uint8> UGRILL_SimulationManagementFamilyPDU::ToBytes()
{
    DIS::DataStream buffer(DIS::BIG);

    //marshal
    DIS::SimulationManagementFamilyPdu simFamilyPDU;

    ToOpenDIS(simFamilyPDU);
    simFamilyPDU.marshal(buffer);

    TArray<uint8> byteArrayOut;
    byteArrayOut.Init(0, buffer.size());
    for (int i = 0; i < buffer.size(); i++) {
        byteArrayOut[i] = buffer[i];
    }
    buffer.clear();

    return byteArrayOut;
}
