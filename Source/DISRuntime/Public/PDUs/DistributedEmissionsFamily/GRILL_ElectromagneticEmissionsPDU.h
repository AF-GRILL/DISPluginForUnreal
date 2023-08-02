// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/ElectromagneticEmissionsPdu.h> 
#include <dis6/ElectromagneticEmissionSystemData.h> 
#include <dis6/ElectromagneticEmissionBeamData.h> 
#include "PDUs/DistributedEmissionsFamily/GRILL_DistributedEmissionsFamilyPDU.h"
#include "GRILL_ElectromagneticEmissionsPDU.generated.h"

UENUM(BlueprintType)
enum class EBeamFunction: uint8
{
	Other,
	Search,
	HeightFinding,
	Acquisition,
	Tracking,
	AcquisitionAndTracking,
	CommandGuidance,
	Illumination,
	Ranging,
	MissileBeacon,
	MissileFusing,
	ActiveRadarMissileSeeker,
	Jamming,
	IFF,
	NavigationWeather,
	Meteorological,
	DataTransmission,
	NavigationalDirectionalBeacon,
	TimeSharedSearch,
	TimeSharedAcquisition,
	TimeSharedTrack,
	TimeSharedCommandGuidance,
	TimeSharedIllumination,
	TimeSharedJamming,
};

USTRUCT(BlueprintType)
struct FElectromagneticEmissionBeamData
{
	GENERATED_BODY()

	/** This field shall specify a unique emitter database number assigned to differentiate between otherwise similar or identical emitter beams within an emitter system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		uint8 BeamIDNumber = 0;
	/** This field shall specify a Beam Parameter Index number that shall be used by receiving entities in conjunction with the Emitter Name field to provide a pointer to the stored database parameters required to regenerate the beam.  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		uint8 BeamParameterIndex = 0;


	/** center frequency of the emission in hertz. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float Frequency = 0;
	/** Bandwidth of the frequencies corresponding to the fequency field. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float FrequencyRange = 0;
	/** Effective radiated power for the emission in DdBm. For a      radar noise jammer, indicates the peak of the transmitted power. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float EffectiveRadiatedPower = 0;
	/** Average repetition frequency of the emission in hertz. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float PulseRepetitionFrequency = 0;
	/** Average pulse width  of the emission in microseconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float PulseWidth = 0;
	/** Specifies the beam azimuth an elevation centers and corresponding half-angles     to describe the scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float BeamAzimuthCenter = 0;
	/** Specifies the beam azimuth sweep to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float BeamAzimuthSweep = 0;
	/** Specifies the beam elevation center to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float BeamElevationCenter = 0;
	/** Specifies the beam elevation sweep to determine scan volume */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float BeamElevationSweep = 0;
	/** allows receiver to synchronize its regenerated scan pattern to     that of the emmitter. Specifies the percentage of time a scan is through its pattern from its origion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		float BeamSweepSync = 0;


	/** beam function of a particular beam */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		EBeamFunction BeamFunction = EBeamFunction::Other;

	void SetupFromOpenDIS(const DIS::ElectromagneticEmissionBeamData& Data)
	{
		BeamIDNumber = Data.getBeamIDNumber();
		BeamParameterIndex = Data.getBeamParameterIndex();

		Frequency = Data.getFundamentalParameterData().getFrequency();
		FrequencyRange = Data.getFundamentalParameterData().getFrequencyRange();
		EffectiveRadiatedPower = Data.getFundamentalParameterData().getEffectiveRadiatedPower();
		PulseRepetitionFrequency = Data.getFundamentalParameterData().getPulseRepetitionFrequency();
		PulseWidth = Data.getFundamentalParameterData().getPulseWidth();
		BeamAzimuthCenter = Data.getFundamentalParameterData().getBeamAzimuthCenter();
		BeamAzimuthSweep = Data.getFundamentalParameterData().getBeamAzimuthSweep();
		BeamElevationCenter = Data.getFundamentalParameterData().getBeamElevationCenter();
		BeamElevationSweep = Data.getFundamentalParameterData().getBeamElevationSweep();

		BeamFunction = static_cast<EBeamFunction>(Data.getBeamFunction());
	}

	void ToOpenDIS(DIS::ElectromagneticEmissionSystemData& out)
	{
	}
};

USTRUCT(BlueprintType)
struct FElectromagneticEmissionSystemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		TArray<FElectromagneticEmissionBeamData> Beams; 

	void SetupFromOpenDIS(const DIS::ElectromagneticEmissionSystemData& Data)
	{
		Beams.Empty();
		for (const auto& BeamIn : Data.getBeamDataRecords()) {
			FElectromagneticEmissionBeamData Beam;
			Beam.SetupFromOpenDIS(BeamIn);
			Beams.Add(Beam);
		}
	}

	void ToOpenDIS(DIS::ElectromagneticEmissionSystemData& out)
	{
	}
};

USTRUCT(BlueprintType)
struct FElectromagneticEmissionsPDU : public FDistributedEmissionsFamilyPDU
{
	GENERATED_BODY()

	/** The site, application, and unique identifier for this entity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		FEntityID EmittingEntityID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		FEventID EventID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		int StateUpdateIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|ElectromagneticEmissions")
		TArray<FElectromagneticEmissionSystemData> Systems;


	FElectromagneticEmissionsPDU() : FDistributedEmissionsFamilyPDU(EPDUType::ElectromagneticEmission)
	{
	}

	void SetupFromOpenDIS(const DIS::ElectromagneticEmissionsPdu& ElectromagneticEmissionsPDUIn)
	{
		FDistributedEmissionsFamilyPDU::SetupFromOpenDIS(ElectromagneticEmissionsPDUIn);
		
		EmittingEntityID = FEntityID(ElectromagneticEmissionsPDUIn.getEmittingEntityID());
		EventID = FEventID(ElectromagneticEmissionsPDUIn.getEventID());

		Systems.Empty();
		for (const auto& SystemIn : ElectromagneticEmissionsPDUIn.getSystems()) {
			FElectromagneticEmissionSystemData System;
			System.SetupFromOpenDIS(SystemIn);
			Systems.Add(System);
		}

	}

	void ToOpenDIS(DIS::ElectromagneticEmissionsPdu& ElectromagneticEmissionsPDUOut)
	{
		FDistributedEmissionsFamilyPDU::ToOpenDIS(ElectromagneticEmissionsPDUOut);
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