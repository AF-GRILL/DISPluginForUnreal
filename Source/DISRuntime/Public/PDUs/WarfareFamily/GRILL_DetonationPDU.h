// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID MunitionEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEventID EventID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EcefLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBurstDescriptor BurstDescriptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LocationInEntityCoords;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 DetonationResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Pad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FArticulationParameters> ArticulationParameters;

	FDetonationPDU() : FWarfareFamilyPDU()
	{
		PduType = EPDUType::Detonation;

		Velocity = FVector(0, 0, 0);
		EcefLocation = FVector(0, 0, 0);
		LocationInEntityCoords = FVector(0, 0, 0);
		DetonationResult = 0U;
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
		DetonationResult = DetonationPDUIn->getDetonationResult();
		Pad = DetonationPDUIn->getPad();

		//Articulation Parameters
		for (int i = 0; i < DetonationPDUIn->getNumberOfArticulationParameters(); i++)
		{
			DIS::ArticulationParameter tempArtParam = DetonationPDUIn->getArticulationParameters()[i];
			FArticulationParameters newArtParam;
			newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
			newArtParam.ParameterType = tempArtParam.getParameterType();
			newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
			newArtParam.ParameterValue = tempArtParam.getParameterValue();
			newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();

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

		DetonationPDUOut.setDetonationResult(DetonationResult);
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