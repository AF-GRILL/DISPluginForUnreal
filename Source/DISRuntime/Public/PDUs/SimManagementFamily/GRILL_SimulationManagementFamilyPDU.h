// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/SimulationManagementFamilyPdu.h>
#include "PDUs/GRILL_PDU.h"
#include "GRILL_SimulationManagementFamilyPDU.generated.h"

USTRUCT(BlueprintType)
struct FSimulationManagementFamilyPDU : public FPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID OriginatingEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID ReceivingEntityID;

    FSimulationManagementFamilyPDU() : FPDU() 
    {

    }

    virtual ~FSimulationManagementFamilyPDU() {}

    void SetupFromOpenDIS(DIS::SimulationManagementFamilyPdu* SimFamilyPDUIn)
    {
        FPDU::SetupFromOpenDIS(SimFamilyPDUIn);

        OriginatingEntityID = SimFamilyPDUIn->getOriginatingEntityID();
        ReceivingEntityID = SimFamilyPDUIn->getReceivingEntityID();
    }

    void ToOpenDIS(DIS::SimulationManagementFamilyPdu& simFamilyPDUOut)
    {
        FPDU::ToOpenDIS(simFamilyPDUOut);

        simFamilyPDUOut.setOriginatingEntityID(OriginatingEntityID.ToOpenDIS());
        simFamilyPDUOut.setReceivingEntityID(ReceivingEntityID.ToOpenDIS());
    }

    virtual TArray<uint8> ToBytes() override
    {
        DIS::DataStream buffer(DIS::BIG);

        //marshal
        DIS::SimulationManagementFamilyPdu simFamilyPDU;

        ToOpenDIS(simFamilyPDU);
        simFamilyPDU.marshal(buffer);

        return FPDU::DISDataStreamToBytes(buffer);
    }
};