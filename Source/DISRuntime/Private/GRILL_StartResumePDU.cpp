// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_StartResumePDU.h"

UGRILL_StartResumePDU::UGRILL_StartResumePDU() : UGRILL_SimulationManagementFamilyPDU()
{
	PDUStruct.PduType = EPDUType::Start_Resume;
}

void UGRILL_StartResumePDU::SetupFromOpenDIS(DIS::StartResumePdu* StartResumePDUIn)
{
	UGRILL_SimulationManagementFamilyPDU::SetupFromOpenDIS(StartResumePDUIn);

	// Start/Resume PDU specific
	DIS::ClockTime tempRealWorldTime = StartResumePDUIn->getRealWorldTime();
	DIS::ClockTime tempSimulationTime = StartResumePDUIn->getRealWorldTime();

	StartResumePDUStruct.RealWorldTime.Hour = tempRealWorldTime.getHour();
	StartResumePDUStruct.RealWorldTime.TimePastHour = tempRealWorldTime.getTimePastHour();

	StartResumePDUStruct.SimulationTime.Hour = tempSimulationTime.getHour();
	StartResumePDUStruct.SimulationTime.TimePastHour = tempSimulationTime.getTimePastHour();

	StartResumePDUStruct.RequestID = StartResumePDUIn->getRequestID();
}

void UGRILL_StartResumePDU::ToOpenDIS(DIS::StartResumePdu& StartResumePDUOut)
{
	UGRILL_SimulationManagementFamilyPDU::ToOpenDIS(StartResumePDUOut);

	//Inherited PDU setup
	StartResumePDUOut.setReceivingEntityID(SimManagementFamilyPDUStruct.ReceivingEntityID.ToOpenDIS());
	StartResumePDUOut.setOriginatingEntityID(SimManagementFamilyPDUStruct.OriginatingEntityID.ToOpenDIS());

	// Specific PDU setup
	StartResumePDUOut.setRealWorldTime(StartResumePDUStruct.RealWorldTime.ToOpenDIS());
	StartResumePDUOut.setSimulationTime(StartResumePDUStruct.SimulationTime.ToOpenDIS());
	StartResumePDUOut.setRequestID(StartResumePDUStruct.RequestID);
}

TArray<uint8> UGRILL_StartResumePDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::StartResumePdu startResumePDU;

	ToOpenDIS(startResumePDU);
	startResumePDU.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
