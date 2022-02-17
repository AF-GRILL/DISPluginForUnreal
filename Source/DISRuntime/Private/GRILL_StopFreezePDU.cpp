// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_StopFreezePDU.h"

UGRILL_StopFreezePDU::UGRILL_StopFreezePDU() : UGRILL_SimulationManagementFamilyPDU()
{
	PDUStruct.PduType = EPDUType::Stop_Freeze;
}

void UGRILL_StopFreezePDU::SetupFromOpenDIS(DIS::StopFreezePdu* StopFreezePDUIn)
{
	UGRILL_SimulationManagementFamilyPDU::SetupFromOpenDIS(StopFreezePDUIn);

	// Stop/Freeze PDU specifics
	DIS::ClockTime tempRealWorldTime = StopFreezePDUIn->getRealWorldTime();

	StopFreezePDUStruct.RealWorldTime.Hour = tempRealWorldTime.getHour();
	StopFreezePDUStruct.RealWorldTime.TimePastHour = tempRealWorldTime.getTimePastHour();

	StopFreezePDUStruct.Reason = static_cast<EReason>(StopFreezePDUIn->getReason());
	StopFreezePDUStruct.FrozenBehavior = StopFreezePDUIn->getFrozenBehavior();
	StopFreezePDUStruct.PaddingOne = StopFreezePDUIn->getPadding1();
	StopFreezePDUStruct.RequestID = StopFreezePDUIn->getRequestID();
}

void UGRILL_StopFreezePDU::ToOpenDIS(DIS::StopFreezePdu& StopFreezePDUOut)
{
	UGRILL_SimulationManagementFamilyPDU::ToOpenDIS(StopFreezePDUOut);

	//Inherited PDU setup
	StopFreezePDUOut.setReceivingEntityID(SimManagementFamilyPDUStruct.ReceivingEntityID.ToOpenDIS());
	StopFreezePDUOut.setOriginatingEntityID(SimManagementFamilyPDUStruct.OriginatingEntityID.ToOpenDIS());

	// Specific PDU setup
	StopFreezePDUOut.setRealWorldTime(StopFreezePDUStruct.RealWorldTime.ToOpenDIS());
	StopFreezePDUOut.setReason(static_cast<unsigned char>(StopFreezePDUStruct.Reason));
	StopFreezePDUOut.setFrozenBehavior(StopFreezePDUStruct.FrozenBehavior);
	StopFreezePDUOut.setPadding1(StopFreezePDUStruct.PaddingOne);
	StopFreezePDUOut.setRequestID(StopFreezePDUStruct.RequestID);
}

TArray<uint8> UGRILL_StopFreezePDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::StopFreezePdu stopFreezePDU;

	ToOpenDIS(stopFreezePDU);
	stopFreezePDU.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
