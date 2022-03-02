// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/StartResumePdu.h>
#include "PDUs/SimManagementFamily/GRILL_SimulationManagementFamilyPDU.h"
#include "GRILL_StartResumePDU.generated.h"

USTRUCT(BlueprintType)
struct FStartResumePDU : public FSimulationManagementFamilyPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime RealWorldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FClockTime SimulationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int64 RequestID;

	FStartResumePDU() : FSimulationManagementFamilyPDU()
	{
		PduType = EPDUType::Start_Resume;

		RequestID = 0;
	}

	virtual ~FStartResumePDU() {}

	void SetupFromOpenDIS(DIS::StartResumePdu* StartResumePDUIn)
	{
		FSimulationManagementFamilyPDU::SetupFromOpenDIS(StartResumePDUIn);

		// Start/Resume PDU specific
		DIS::ClockTime tempRealWorldTime = StartResumePDUIn->getRealWorldTime();
		DIS::ClockTime tempSimulationTime = StartResumePDUIn->getRealWorldTime();

		RealWorldTime.Hour = tempRealWorldTime.getHour();
		RealWorldTime.TimePastHour = tempRealWorldTime.getTimePastHour();

		SimulationTime.Hour = tempSimulationTime.getHour();
		SimulationTime.TimePastHour = tempSimulationTime.getTimePastHour();

		RequestID = StartResumePDUIn->getRequestID();
	}

	void ToOpenDIS(DIS::StartResumePdu& StartResumePDUOut)
	{
		FSimulationManagementFamilyPDU::ToOpenDIS(StartResumePDUOut);

		//Inherited PDU setup
		StartResumePDUOut.setReceivingEntityID(ReceivingEntityID.ToOpenDIS());
		StartResumePDUOut.setOriginatingEntityID(OriginatingEntityID.ToOpenDIS());

		// Specific PDU setup
		StartResumePDUOut.setRealWorldTime(RealWorldTime.ToOpenDIS());
		StartResumePDUOut.setSimulationTime(SimulationTime.ToOpenDIS());
		StartResumePDUOut.setRequestID(RequestID);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::StartResumePdu startResumePDU;

		ToOpenDIS(startResumePDU);
		startResumePDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};