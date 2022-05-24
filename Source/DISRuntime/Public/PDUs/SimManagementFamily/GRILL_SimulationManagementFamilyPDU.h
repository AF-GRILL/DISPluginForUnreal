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

    /** The site,  application, and reference number of the originating simulation or entity. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateFamily")
        FEntityID OriginatingEntityID;
    /** The site, application, and reference number of the receiving simulation or entity. This can be used to filter packets to the correct simulations/entities */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateFamily")
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