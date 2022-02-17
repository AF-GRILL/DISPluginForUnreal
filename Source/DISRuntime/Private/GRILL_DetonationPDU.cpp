// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_DetonationPDU.h"

UGRILL_DetonationPDU::UGRILL_DetonationPDU() : UGRILL_WarfareFamilyPDU()
{
	PDUStruct.PduType = EPDUType::Detonation;
}

void UGRILL_DetonationPDU::SetupFromOpenDIS(DIS::DetonationPdu* DetonationPDUIn)
{
	UGRILL_WarfareFamilyPDU::SetupFromOpenDIS(DetonationPDUIn);

	//Detonation PDU specifics
	//MunitionEntityID
	DetonationPDUStruct.MunitionEntityID = DetonationPDUIn->getMunitionID();

	//event id
	DetonationPDUStruct.EventID = DetonationPDUIn->getEventID();

	//velocity
	DetonationPDUStruct.Velocity[0] = DetonationPDUIn->getVelocity().getX();
	DetonationPDUStruct.Velocity[1] = DetonationPDUIn->getVelocity().getY();
	DetonationPDUStruct.Velocity[2] = DetonationPDUIn->getVelocity().getZ();

	//location
	DetonationPDUStruct.Location[0] = DetonationPDUIn->getLocationInWorldCoordinates().getX();
	DetonationPDUStruct.Location[1] = DetonationPDUIn->getLocationInWorldCoordinates().getY();
	DetonationPDUStruct.Location[2] = DetonationPDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	DetonationPDUStruct.LocationDouble[0] = DetonationPDUIn->getLocationInWorldCoordinates().getX();
	DetonationPDUStruct.LocationDouble[1] = DetonationPDUIn->getLocationInWorldCoordinates().getY();
	DetonationPDUStruct.LocationDouble[2] = DetonationPDUIn->getLocationInWorldCoordinates().getZ();

	//location
	DetonationPDUStruct.LocationInEntityCoords[0] = DetonationPDUIn->getLocationInEntityCoordinates().getX();
	DetonationPDUStruct.LocationInEntityCoords[1] = DetonationPDUIn->getLocationInEntityCoordinates().getY();
	DetonationPDUStruct.LocationInEntityCoords[2] = DetonationPDUIn->getLocationInEntityCoordinates().getZ();

	//burst descriptor
	DetonationPDUStruct.BurstDescriptor.Warhead = DetonationPDUIn->getBurstDescriptor().getWarhead();
	DetonationPDUStruct.BurstDescriptor.Fuse = DetonationPDUIn->getBurstDescriptor().getFuse();
	DetonationPDUStruct.BurstDescriptor.Rate = DetonationPDUIn->getBurstDescriptor().getRate();
	DetonationPDUStruct.BurstDescriptor.Quantity = DetonationPDUIn->getBurstDescriptor().getQuantity();
	DetonationPDUStruct.BurstDescriptor.EntityType = DetonationPDUIn->getBurstDescriptor().getMunition();

	//single vars
	DetonationPDUStruct.DetonationResult = DetonationPDUIn->getDetonationResult();
	DetonationPDUStruct.Pad = DetonationPDUIn->getPad();

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

		DetonationPDUStruct.ArticulationParameters.Add(newArtParam);
	}
}

void UGRILL_DetonationPDU::ToOpenDIS(DIS::DetonationPdu& DetonationPDUOut)
{
	UGRILL_WarfareFamilyPDU::ToOpenDIS(DetonationPDUOut);

	//Inherited PDU setup
	DetonationPDUOut.setFiringEntityID(WarfareFamilyPDUStruct.FiringEntityID.ToOpenDIS());
	DetonationPDUOut.setTargetEntityID(WarfareFamilyPDUStruct.TargetEntityID.ToOpenDIS());

	// Specific PDU setup
	DetonationPDUOut.setEventID(DetonationPDUStruct.EventID.ToOpenDIS());
	DetonationPDUOut.setMunitionID(DetonationPDUStruct.MunitionEntityID.ToOpenDIS());

	DIS::Vector3Float OutVelocity;
	OutVelocity.setX(DetonationPDUStruct.Velocity.X);
	OutVelocity.setY(DetonationPDUStruct.Velocity.Y);
	OutVelocity.setZ(DetonationPDUStruct.Velocity.Z);
	DetonationPDUOut.setVelocity(OutVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(DetonationPDUStruct.LocationDouble[0]), DetonationPDUStruct.Location.X) &&
		FMath::IsNearlyEqual(static_cast<float>(DetonationPDUStruct.LocationDouble[1]), DetonationPDUStruct.Location.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(DetonationPDUStruct.LocationDouble[2]), DetonationPDUStruct.Location.Z))
	{
		OutLocation.setX(DetonationPDUStruct.LocationDouble[0]);
		OutLocation.setY(DetonationPDUStruct.LocationDouble[1]);
		OutLocation.setZ(DetonationPDUStruct.LocationDouble[2]);
	}
	else
	{
		OutLocation.setX(DetonationPDUStruct.Location.X);
		OutLocation.setY(DetonationPDUStruct.Location.Y);
		OutLocation.setZ(DetonationPDUStruct.Location.Z);
	}
	DetonationPDUOut.setLocationInWorldCoordinates(OutLocation);

	DIS::Vector3Float OutLocationInEntityCoords;
	OutLocationInEntityCoords.setX(DetonationPDUStruct.LocationInEntityCoords.X);
	OutLocationInEntityCoords.setY(DetonationPDUStruct.LocationInEntityCoords.Y);
	OutLocationInEntityCoords.setZ(DetonationPDUStruct.LocationInEntityCoords.Z);
	DetonationPDUOut.setLocationInEntityCoordinates(OutLocationInEntityCoords);

	DetonationPDUOut.setDetonationResult(DetonationPDUStruct.DetonationResult);
	DetonationPDUOut.setPad(DetonationPDUStruct.Pad);

	std::vector<DIS::ArticulationParameter> OutArtParams;
	for (FArticulationParameters ArticulationParameter : DetonationPDUStruct.ArticulationParameters)
	{
		OutArtParams.push_back(ArticulationParameter.ToOpenDIS());
	}
	DetonationPDUOut.setArticulationParameters(OutArtParams);
}

TArray<uint8> UGRILL_DetonationPDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::DetonationPdu detPDU;

	ToOpenDIS(detPDU);
	detPDU.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
