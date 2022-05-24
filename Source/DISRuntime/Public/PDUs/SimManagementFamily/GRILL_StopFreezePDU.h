// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/StopFreezePdu.h>
#include "PDUs/SimManagementFamily/GRILL_SimulationManagementFamilyPDU.h"
#include "GRILL_StopFreezePDU.generated.h"

USTRUCT(BlueprintType)
struct FStopFreezePDU : public FSimulationManagementFamilyPDU
{
	GENERATED_BODY()

	/** The  real-world time at which the entity/exercise is to start/resume in the exercise. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|StopFreeze")
		FClockTime RealWorldTime;
	/** The reason that an entity or exercise was stopped/frozen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|StopFreeze")
		EReason Reason;
	/** Specification of the internal behavior of the simulation and its appearance while frozen to other participants of the exercise */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|StopFreeze")
		int32 FrozenBehavior;
	/** Unused padding */
	UPROPERTY()
		int32 PaddingOne;
	/** The specific and unique stop/freeze request being made by the simulation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|StopFreeze")
		int64 RequestID;

	FStopFreezePDU() : FSimulationManagementFamilyPDU()
	{
		PduType = EPDUType::Stop_Freeze;

		Reason = EReason::Other;
		FrozenBehavior = 0;
		PaddingOne = 0;
		RequestID = 0;
	}

	virtual ~FStopFreezePDU() {}

	void SetupFromOpenDIS(DIS::StopFreezePdu* StopFreezePDUIn)
	{
		FSimulationManagementFamilyPDU::SetupFromOpenDIS(StopFreezePDUIn);

		// Stop/Freeze PDU specifics
		DIS::ClockTime tempRealWorldTime = StopFreezePDUIn->getRealWorldTime();

		RealWorldTime.Hour = tempRealWorldTime.getHour();
		RealWorldTime.TimePastHour = tempRealWorldTime.getTimePastHour();

		Reason = static_cast<EReason>(StopFreezePDUIn->getReason());
		FrozenBehavior = StopFreezePDUIn->getFrozenBehavior();
		PaddingOne = StopFreezePDUIn->getPadding1();
		RequestID = StopFreezePDUIn->getRequestID();
	}

	void ToOpenDIS(DIS::StopFreezePdu& StopFreezePDUOut)
	{
		FSimulationManagementFamilyPDU::ToOpenDIS(StopFreezePDUOut);

		//Inherited PDU setup
		StopFreezePDUOut.setReceivingEntityID(ReceivingEntityID.ToOpenDIS());
		StopFreezePDUOut.setOriginatingEntityID(OriginatingEntityID.ToOpenDIS());

		// Specific PDU setup
		StopFreezePDUOut.setRealWorldTime(RealWorldTime.ToOpenDIS());
		StopFreezePDUOut.setReason(static_cast<unsigned char>(Reason));
		StopFreezePDUOut.setFrozenBehavior(FrozenBehavior);
		StopFreezePDUOut.setPadding1(PaddingOne);
		StopFreezePDUOut.setRequestID(RequestID);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::StopFreezePdu stopFreezePDU;

		ToOpenDIS(stopFreezePDU);
		stopFreezePDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};