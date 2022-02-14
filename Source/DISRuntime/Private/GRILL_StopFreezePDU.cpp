// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_StopFreezePDU.h"

UGRILL_StopFreezePDU::UGRILL_StopFreezePDU()
{
	StopFreezePduStruct.PduType = EPDUType::Stop_Freeze;
	StopFreezePduStruct.Reason = EReason::Other;
	StopFreezePduStruct.FrozenBehavior = 0;
	StopFreezePduStruct.PaddingOne = 0;
	StopFreezePduStruct.RequestID = 0;
}

void UGRILL_StopFreezePDU::SetupFromOpenDIS(DIS::StopFreezePdu* StopFreezePDUIn)
{
	//pdu common parameters
	StopFreezePduStruct.ProtocolVersion = StopFreezePDUIn->getProtocolVersion();
	StopFreezePduStruct.ExerciseID = StopFreezePDUIn->getExerciseID();
	StopFreezePduStruct.PduType = static_cast<EPDUType>(StopFreezePDUIn->getPduType());
	StopFreezePduStruct.ProtocolFamily = StopFreezePDUIn->getProtocolFamily();
	StopFreezePduStruct.Timestamp = StopFreezePDUIn->getTimestamp();
	StopFreezePduStruct.Length = StopFreezePDUIn->getLength();
	StopFreezePduStruct.Padding = StopFreezePDUIn->getPadding();

	//Simulation Management Family Pdu specific
	StopFreezePduStruct.OriginatingEntityID = StopFreezePDUIn->getOriginatingEntityID();
	StopFreezePduStruct.ReceivingEntityID = StopFreezePDUIn->getReceivingEntityID();

	// Stop/Freeze PDU specifics
	DIS::ClockTime tempRealWorldTime = StopFreezePDUIn->getRealWorldTime();

	StopFreezePduStruct.RealWorldTime.Hour = tempRealWorldTime.getHour();
	StopFreezePduStruct.RealWorldTime.TimePastHour = tempRealWorldTime.getTimePastHour();

	StopFreezePduStruct.Reason = static_cast<EReason>(StopFreezePDUIn->getReason());
	StopFreezePduStruct.FrozenBehavior = StopFreezePDUIn->getFrozenBehavior();
	StopFreezePduStruct.PaddingOne = StopFreezePDUIn->getPadding1();
	StopFreezePduStruct.RequestID = StopFreezePDUIn->getRequestID();
}

DIS::StopFreezePdu UGRILL_StopFreezePDU::ToOpenDIS()
{
	DIS::StopFreezePdu stopFreezePDUOut;

	// Common PDU setup
	stopFreezePDUOut.setProtocolVersion(StopFreezePduStruct.ProtocolVersion);
	stopFreezePDUOut.setExerciseID(StopFreezePduStruct.ExerciseID);
	stopFreezePDUOut.setPduType(static_cast<unsigned char>(StopFreezePduStruct.PduType));
	stopFreezePDUOut.setProtocolFamily(StopFreezePduStruct.ProtocolFamily);
	stopFreezePDUOut.setTimestamp(StopFreezePduStruct.Timestamp);
	stopFreezePDUOut.setLength(StopFreezePduStruct.Length);
	stopFreezePDUOut.setPadding(StopFreezePduStruct.Padding);

	// Simulation Management Family PDU setup
	stopFreezePDUOut.setOriginatingEntityID(StopFreezePduStruct.OriginatingEntityID.ToOpenDIS());
	stopFreezePDUOut.setReceivingEntityID(StopFreezePduStruct.ReceivingEntityID.ToOpenDIS());

	// Specific PDU setup
	stopFreezePDUOut.setReceivingEntityID(StopFreezePduStruct.ReceivingEntityID.ToOpenDIS());
	stopFreezePDUOut.setOriginatingEntityID(StopFreezePduStruct.OriginatingEntityID.ToOpenDIS());
	stopFreezePDUOut.setRealWorldTime(StopFreezePduStruct.RealWorldTime.ToOpenDIS());
	stopFreezePDUOut.setReason(static_cast<unsigned char>(StopFreezePduStruct.Reason));
	stopFreezePDUOut.setFrozenBehavior(StopFreezePduStruct.FrozenBehavior);
	stopFreezePDUOut.setPadding1(StopFreezePduStruct.PaddingOne);
	stopFreezePDUOut.setRequestID(StopFreezePduStruct.RequestID);

	return stopFreezePDUOut;
}

TArray<uint8> UGRILL_StopFreezePDU::ToBytes()
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
