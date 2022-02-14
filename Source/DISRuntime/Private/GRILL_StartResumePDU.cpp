// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_StartResumePDU.h"

UGRILL_StartResumePDU::UGRILL_StartResumePDU()
{
	StartResumePduStruct.PduType = EPDUType::Start_Resume;
	StartResumePduStruct.RequestID = 0;
}

void UGRILL_StartResumePDU::SetupFromOpenDIS(DIS::StartResumePdu* StartResumePDUIn)
{
	//pdu common parameters
	StartResumePduStruct.ProtocolVersion = StartResumePDUIn->getProtocolVersion();
	StartResumePduStruct.ExerciseID = StartResumePDUIn->getExerciseID();
	StartResumePduStruct.PduType = static_cast<EPDUType>(StartResumePDUIn->getPduType());
	StartResumePduStruct.ProtocolFamily = StartResumePDUIn->getProtocolFamily();
	StartResumePduStruct.Timestamp = StartResumePDUIn->getTimestamp();
	StartResumePduStruct.Length = StartResumePDUIn->getLength();
	StartResumePduStruct.Padding = StartResumePDUIn->getPadding();

	//Simulation Management Family Pdu specific
	StartResumePduStruct.OriginatingEntityID = StartResumePDUIn->getOriginatingEntityID();
	StartResumePduStruct.ReceivingEntityID = StartResumePDUIn->getReceivingEntityID();

	// Start/Resume PDU specific
	DIS::ClockTime tempRealWorldTime = StartResumePDUIn->getRealWorldTime();
	DIS::ClockTime tempSimulationTime = StartResumePDUIn->getRealWorldTime();

	StartResumePduStruct.RealWorldTime.Hour = tempRealWorldTime.getHour();
	StartResumePduStruct.RealWorldTime.TimePastHour = tempRealWorldTime.getTimePastHour();

	StartResumePduStruct.SimulationTime.Hour = tempSimulationTime.getHour();
	StartResumePduStruct.SimulationTime.TimePastHour = tempSimulationTime.getTimePastHour();

	StartResumePduStruct.RequestID = StartResumePDUIn->getRequestID();
}

DIS::StartResumePdu UGRILL_StartResumePDU::ToOpenDIS()
{
	DIS::StartResumePdu startResumePDUOut;

	// Common PDU setup
	startResumePDUOut.setProtocolVersion(StartResumePduStruct.ProtocolVersion);
	startResumePDUOut.setExerciseID(StartResumePduStruct.ExerciseID);
	startResumePDUOut.setPduType(static_cast<unsigned char>(StartResumePduStruct.PduType));
	startResumePDUOut.setProtocolFamily(StartResumePduStruct.ProtocolFamily);
	startResumePDUOut.setTimestamp(StartResumePduStruct.Timestamp);
	startResumePDUOut.setLength(StartResumePduStruct.Length);
	startResumePDUOut.setPadding(StartResumePduStruct.Padding);

	// Simulation Management Family PDU setup
	startResumePDUOut.setOriginatingEntityID(StartResumePduStruct.OriginatingEntityID.ToOpenDIS());
	startResumePDUOut.setReceivingEntityID(StartResumePduStruct.ReceivingEntityID.ToOpenDIS());

	// Specific PDU setup
	startResumePDUOut.setReceivingEntityID(StartResumePduStruct.ReceivingEntityID.ToOpenDIS());
	startResumePDUOut.setOriginatingEntityID(StartResumePduStruct.OriginatingEntityID.ToOpenDIS());
	startResumePDUOut.setRealWorldTime(StartResumePduStruct.RealWorldTime.ToOpenDIS());
	startResumePDUOut.setSimulationTime(StartResumePduStruct.SimulationTime.ToOpenDIS());
	startResumePDUOut.setRequestID(StartResumePduStruct.RequestID);

	return startResumePDUOut;
}

TArray<uint8> UGRILL_StartResumePDU::ToBytes()
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
