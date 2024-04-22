// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/ElectromagneticEmissionsPdu.h> 
#include "PDUs/DistributedEmissionsFamily/GRILL_DistributedEmissionsFamilyPDU.h"
#include "GRILL_ElectromagneticEmissionsPDU.generated.h"

USTRUCT(BlueprintType)
struct FElectromagneticEmissionsPDU : public FDistributedEmissionsFamilyPDU
{
	GENERATED_BODY()

	/** The site, application, and unique identifier for this entity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		FEntityID EmittingEntityID;
	/** This field shall contain and identification by the issuing simulation application to associate related events. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		FEventID EventID;
	/** This field shall be used to indicate if the data in the PDU represents a state update or just data that has changed since issuance of the last Emitter PDU. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		EStateUpdateIndicator StateUpdateIndicator;
	/** Padding */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		int32 PaddingForEmissionsPDU;
	/** Electromagnetic emmissions systems */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		TArray<FElectromagneticEmissionSystemData> Systems;

	FElectromagneticEmissionsPDU() : FDistributedEmissionsFamilyPDU()
	{
		PduType = EPDUType::ElectromagneticEmission;

		StateUpdateIndicator = EStateUpdateIndicator::HeartbeatUpdate;
		PaddingForEmissionsPDU = 0;
	}

	void SetupFromOpenDIS(const DIS::ElectromagneticEmissionsPdu& ElectromagneticEmissionsPDUIn)
	{
		FDistributedEmissionsFamilyPDU::SetupFromOpenDIS(ElectromagneticEmissionsPDUIn);
		
		EmittingEntityID = ElectromagneticEmissionsPDUIn.getEmittingEntityID();
		EventID = ElectromagneticEmissionsPDUIn.getEventID();
		StateUpdateIndicator = static_cast<EStateUpdateIndicator>(ElectromagneticEmissionsPDUIn.getStateUpdateIndicator());
		PaddingForEmissionsPDU = ElectromagneticEmissionsPDUIn.getPaddingForEmissionsPdu();

		Systems.Empty();
		for (int i = 0; i < ElectromagneticEmissionsPDUIn.getNumberOfSystems(); i++)
		{
			FElectromagneticEmissionSystemData System;
			System.SetupFromOpenDIS(ElectromagneticEmissionsPDUIn.getSystems()[i]);
			Systems.Add(System);
		}
	}

	void ToOpenDIS(DIS::ElectromagneticEmissionsPdu& ElectromagneticEmissionsPDUOut)
	{
		FDistributedEmissionsFamilyPDU::ToOpenDIS(ElectromagneticEmissionsPDUOut);

		ElectromagneticEmissionsPDUOut.setEmittingEntityID(EmittingEntityID.ToOpenDIS());
		ElectromagneticEmissionsPDUOut.setEventID(EventID.ToOpenDIS());
		ElectromagneticEmissionsPDUOut.setStateUpdateIndicator(static_cast<unsigned char>(StateUpdateIndicator));
		ElectromagneticEmissionsPDUOut.setPaddingForEmissionsPdu(PaddingForEmissionsPDU);

		std::vector<DIS::ElectromagneticEmissionSystemData> outSystems;
		for (FElectromagneticEmissionSystemData Param : Systems)
		{
			outSystems.push_back(Param.ToOpenDIS());
		}
		ElectromagneticEmissionsPDUOut.setSystems(outSystems);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::ElectromagneticEmissionsPdu espdu;

		ToOpenDIS(espdu);
		espdu.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};