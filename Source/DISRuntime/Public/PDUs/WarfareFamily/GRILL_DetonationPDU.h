// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/DetonationPdu.h>
#include "PDUs/WarfareFamily/GRILL_WarfareFamilyPDU.h"
#include "GRILL_DetonationPDU.generated.h"

USTRUCT(BlueprintType)
struct FDetonationPDU : public FWarfareFamilyPDU
{
	GENERATED_BODY()

	/** The ID of the munition or expendable entity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		FEntityID MunitionEntityID;
	/** The event ID from the originating Fire PDU if one exist, otherwise the event number shall be 0. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		FEventID EventID;
	/**
	* The velocity of the munition immediately before detonation/impact, the velocity of a non-munition entity
	* immediately before exploding, or the velocity of an expendable immediately before a chaff burst or ignition
	* of a flare. Velocity is represented in world coordinates in meters per second.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		FVector Velocity;
	/** The location of the detonation in world coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		FVector EcefLocation;
	/** Description of the detonation in the form of a munition descriptor, an explosion descriptor, or an expendable descriptor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		FBurstDescriptor BurstDescriptor;
	/** The location of the munition detonation, the expendaable detonation, or the non-munition explosion in the entity coordinate system (right-hand system) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		FVector LocationInEntityCoords;
	/** The result of the detonation as denoted in SISO-REF-010-2015 UID 62 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		EDetonationResult DetonationResult;
	/** Unused padding */
	UPROPERTY()
		int32 Pad;
	/** A set of parameter values for each variable parameter record that is included. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Detonation")
		TArray<FArticulationParameters> ArticulationParameters;

	FDetonationPDU() : FWarfareFamilyPDU()
	{
		PduType = EPDUType::Detonation;

		Velocity = FVector(0, 0, 0);
		EcefLocation = FVector(0, 0, 0);
		LocationInEntityCoords = FVector(0, 0, 0);
		DetonationResult = EDetonationResult::Other;
		Pad = 0;
	}

	virtual ~FDetonationPDU() {}

	void SetupFromOpenDIS(DIS::DetonationPdu* DetonationPDUIn)
	{
		FWarfareFamilyPDU::SetupFromOpenDIS(DetonationPDUIn);

		//Detonation PDU specifics
		//MunitionEntityID
		MunitionEntityID = DetonationPDUIn->getMunitionID();

		//event id
		EventID = DetonationPDUIn->getEventID();

		//velocity
		Velocity[0] = DetonationPDUIn->getVelocity().getX();
		Velocity[1] = DetonationPDUIn->getVelocity().getY();
		Velocity[2] = DetonationPDUIn->getVelocity().getZ();

		//location
		EcefLocation[0] = DetonationPDUIn->getLocationInWorldCoordinates().getX();
		EcefLocation[1] = DetonationPDUIn->getLocationInWorldCoordinates().getY();
		EcefLocation[2] = DetonationPDUIn->getLocationInWorldCoordinates().getZ();

		//location
		LocationInEntityCoords[0] = DetonationPDUIn->getLocationInEntityCoordinates().getX();
		LocationInEntityCoords[1] = DetonationPDUIn->getLocationInEntityCoordinates().getY();
		LocationInEntityCoords[2] = DetonationPDUIn->getLocationInEntityCoordinates().getZ();

		//burst descriptor
		BurstDescriptor.Warhead = DetonationPDUIn->getBurstDescriptor().getWarhead();
		BurstDescriptor.Fuse = DetonationPDUIn->getBurstDescriptor().getFuse();
		BurstDescriptor.Rate = DetonationPDUIn->getBurstDescriptor().getRate();
		BurstDescriptor.Quantity = DetonationPDUIn->getBurstDescriptor().getQuantity();
		BurstDescriptor.EntityType = DetonationPDUIn->getBurstDescriptor().getMunition();

		//single vars
		DetonationResult = static_cast<EDetonationResult>(DetonationPDUIn->getDetonationResult());
		Pad = DetonationPDUIn->getPad();

		//Articulation Parameters
		for (int i = 0; i < DetonationPDUIn->getNumberOfArticulationParameters(); i++)
		{
			DIS::ArticulationParameter tempArtParam = DetonationPDUIn->getArticulationParameters()[i];
			FArticulationParameters newArtParam;
			newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
			newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
			newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();
			newArtParam.ParameterType = tempArtParam.getParameterType();

			if (newArtParam.ParameterTypeDesignator == 0)
			{
				newArtParam.ParameterValue = tempArtParam.getParameterValue();
			}
			else
			{
				newArtParam.AttachedPartType = tempArtParam.getParameterValue();
			}

			ArticulationParameters.Add(newArtParam);
		}
	}

	void ToOpenDIS(DIS::DetonationPdu& DetonationPDUOut)
	{
		FWarfareFamilyPDU::ToOpenDIS(DetonationPDUOut);

		//Inherited PDU setup
		DetonationPDUOut.setFiringEntityID(FiringEntityID.ToOpenDIS());
		DetonationPDUOut.setTargetEntityID(TargetEntityID.ToOpenDIS());

		// Specific PDU setup
		DetonationPDUOut.setEventID(EventID.ToOpenDIS());
		DetonationPDUOut.setMunitionID(MunitionEntityID.ToOpenDIS());

		DIS::Vector3Float OutVelocity;
		OutVelocity.setX(Velocity.X);
		OutVelocity.setY(Velocity.Y);
		OutVelocity.setZ(Velocity.Z);
		DetonationPDUOut.setVelocity(OutVelocity);

		DIS::Vector3Double OutLocation;
		OutLocation.setX(EcefLocation[0]);
		OutLocation.setY(EcefLocation[1]);
		OutLocation.setZ(EcefLocation[2]);
		DetonationPDUOut.setLocationInWorldCoordinates(OutLocation);

		DIS::Vector3Float OutLocationInEntityCoords;
		OutLocationInEntityCoords.setX(LocationInEntityCoords.X);
		OutLocationInEntityCoords.setY(LocationInEntityCoords.Y);
		OutLocationInEntityCoords.setZ(LocationInEntityCoords.Z);
		DetonationPDUOut.setLocationInEntityCoordinates(OutLocationInEntityCoords);

		DetonationPDUOut.setDetonationResult(static_cast<unsigned char>(DetonationResult));
		DetonationPDUOut.setPad(Pad);

		std::vector<DIS::ArticulationParameter> OutArtParams;
		for (FArticulationParameters ArticulationParameter : ArticulationParameters)
		{
			OutArtParams.push_back(ArticulationParameter.ToOpenDIS());
		}
		DetonationPDUOut.setArticulationParameters(OutArtParams);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::DetonationPdu detPDU;

		ToOpenDIS(detPDU);
		detPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};