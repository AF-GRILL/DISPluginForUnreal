// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_DetonationPDU.h"

UGRILL_DetonationPDU::UGRILL_DetonationPDU()
{
	DetonationPduStruct.PduType = EPDUType::Detonation;
	DetonationPduStruct.Velocity = FVector(0, 0, 0);
	DetonationPduStruct.LocationDouble.Init(0, 3);
	DetonationPduStruct.Location = FVector(0, 0, 0);
	DetonationPduStruct.LocationInEntityCoords = FVector(0, 0, 0);
	DetonationPduStruct.DetonationResult = 0U;
	DetonationPduStruct.Pad = 0;
}

void UGRILL_DetonationPDU::SetupFromOpenDIS(DIS::DetonationPdu* DetonationPDUIn)
{
	//pdu common parameters
	DetonationPduStruct.ProtocolVersion = DetonationPDUIn->getProtocolVersion();
	DetonationPduStruct.ExerciseID = DetonationPDUIn->getExerciseID();
	DetonationPduStruct.PduType = static_cast<EPDUType>(DetonationPDUIn->getPduType());
	DetonationPduStruct.ProtocolFamily = DetonationPDUIn->getProtocolFamily();
	DetonationPduStruct.Timestamp = DetonationPDUIn->getTimestamp();
	DetonationPduStruct.Length = DetonationPDUIn->getLength();
	DetonationPduStruct.Padding = DetonationPDUIn->getPadding();

	// WarfareFamilyPdu specific parameters
	DetonationPduStruct.FiringEntityID = DetonationPDUIn->getFiringEntityID();
	DetonationPduStruct.TargetEntityID = DetonationPDUIn->getTargetEntityID();

	//Detonation PDU specifics
	//MunitionEntityID
	DetonationPduStruct.MunitionEntityID = DetonationPDUIn->getMunitionID();

	//event id
	DetonationPduStruct.EventID = DetonationPDUIn->getEventID();

	//velocity
	DetonationPduStruct.Velocity[0] = DetonationPDUIn->getVelocity().getX();
	DetonationPduStruct.Velocity[1] = DetonationPDUIn->getVelocity().getY();
	DetonationPduStruct.Velocity[2] = DetonationPDUIn->getVelocity().getZ();

	//location
	DetonationPduStruct.Location[0] = DetonationPDUIn->getLocationInWorldCoordinates().getX();
	DetonationPduStruct.Location[1] = DetonationPDUIn->getLocationInWorldCoordinates().getY();
	DetonationPduStruct.Location[2] = DetonationPDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	DetonationPduStruct.LocationDouble[0] = DetonationPDUIn->getLocationInWorldCoordinates().getX();
	DetonationPduStruct.LocationDouble[1] = DetonationPDUIn->getLocationInWorldCoordinates().getY();
	DetonationPduStruct.LocationDouble[2] = DetonationPDUIn->getLocationInWorldCoordinates().getZ();

	//location
	DetonationPduStruct.LocationInEntityCoords[0] = DetonationPDUIn->getLocationInEntityCoordinates().getX();
	DetonationPduStruct.LocationInEntityCoords[1] = DetonationPDUIn->getLocationInEntityCoordinates().getY();
	DetonationPduStruct.LocationInEntityCoords[2] = DetonationPDUIn->getLocationInEntityCoordinates().getZ();

	//burst descriptor
	DetonationPduStruct.BurstDescriptor.Warhead = DetonationPDUIn->getBurstDescriptor().getWarhead();
	DetonationPduStruct.BurstDescriptor.Fuse = DetonationPDUIn->getBurstDescriptor().getFuse();
	DetonationPduStruct.BurstDescriptor.Rate = DetonationPDUIn->getBurstDescriptor().getRate();
	DetonationPduStruct.BurstDescriptor.Quantity = DetonationPDUIn->getBurstDescriptor().getQuantity();
	DetonationPduStruct.BurstDescriptor.EntityType = DetonationPDUIn->getBurstDescriptor().getMunition();

	DetonationPduStruct.TargetEntityID = DetonationPDUIn->getTargetEntityID();

	DetonationPduStruct.FiringEntityID.Site = DetonationPDUIn->getFiringEntityID().getSite();
	DetonationPduStruct.FiringEntityID.Application = DetonationPDUIn->getFiringEntityID().getApplication();
	DetonationPduStruct.FiringEntityID.Entity = DetonationPDUIn->getFiringEntityID().getEntity();

	//single vars
	DetonationPduStruct.PduType = static_cast<EPDUType>(DetonationPDUIn->getPduType());
	DetonationPduStruct.DetonationResult = DetonationPDUIn->getDetonationResult();
	DetonationPduStruct.Pad = DetonationPDUIn->getPad();

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

		DetonationPduStruct.ArticulationParameters.Add(newArtParam);
	}
}

DIS::DetonationPdu UGRILL_DetonationPDU::ToOpenDIS()
{
	DIS::DetonationPdu detonationPDUOut;

	// Common PDU setup
	detonationPDUOut.setProtocolVersion(DetonationPduStruct.ProtocolVersion);
	detonationPDUOut.setExerciseID(DetonationPduStruct.ExerciseID);
	detonationPDUOut.setPduType(static_cast<unsigned char>(DetonationPduStruct.PduType));
	detonationPDUOut.setProtocolFamily(DetonationPduStruct.ProtocolFamily);
	detonationPDUOut.setTimestamp(DetonationPduStruct.Timestamp);
	detonationPDUOut.setLength(DetonationPduStruct.Length);
	detonationPDUOut.setPadding(DetonationPduStruct.Padding);

	// Warfare Family PDU setup
	detonationPDUOut.setFiringEntityID(DetonationPduStruct.FiringEntityID.ToOpenDIS());
	detonationPDUOut.setTargetEntityID(DetonationPduStruct.TargetEntityID.ToOpenDIS());

	// Specific PDU setup
	detonationPDUOut.setEventID(DetonationPduStruct.EventID.ToOpenDIS());
	detonationPDUOut.setMunitionID(DetonationPduStruct.MunitionEntityID.ToOpenDIS());

	DIS::Vector3Float OutVelocity;
	OutVelocity.setX(DetonationPduStruct.Velocity.X);
	OutVelocity.setY(DetonationPduStruct.Velocity.Y);
	OutVelocity.setZ(DetonationPduStruct.Velocity.Z);
	detonationPDUOut.setVelocity(OutVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(DetonationPduStruct.LocationDouble[0]), DetonationPduStruct.Location.X) &&
		FMath::IsNearlyEqual(static_cast<float>(DetonationPduStruct.LocationDouble[1]), DetonationPduStruct.Location.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(DetonationPduStruct.LocationDouble[2]), DetonationPduStruct.Location.Z))
	{
		OutLocation.setX(DetonationPduStruct.LocationDouble[0]);
		OutLocation.setY(DetonationPduStruct.LocationDouble[1]);
		OutLocation.setZ(DetonationPduStruct.LocationDouble[2]);
	}
	else
	{
		OutLocation.setX(DetonationPduStruct.Location.X);
		OutLocation.setY(DetonationPduStruct.Location.Y);
		OutLocation.setZ(DetonationPduStruct.Location.Z);
	}
	detonationPDUOut.setLocationInWorldCoordinates(OutLocation);

	DIS::Vector3Float OutLocationInEntityCoords;
	OutLocationInEntityCoords.setX(DetonationPduStruct.LocationInEntityCoords.X);
	OutLocationInEntityCoords.setY(DetonationPduStruct.LocationInEntityCoords.Y);
	OutLocationInEntityCoords.setZ(DetonationPduStruct.LocationInEntityCoords.Z);
	detonationPDUOut.setLocationInEntityCoordinates(OutLocationInEntityCoords);

	detonationPDUOut.setDetonationResult(DetonationPduStruct.DetonationResult);
	detonationPDUOut.setPad(DetonationPduStruct.Pad);
	detonationPDUOut.setFiringEntityID(DetonationPduStruct.FiringEntityID.ToOpenDIS());
	detonationPDUOut.setTargetEntityID(DetonationPduStruct.TargetEntityID.ToOpenDIS());

	std::vector<DIS::ArticulationParameter> OutArtParams;
	for (FArticulationParameters ArticulationParameter : DetonationPduStruct.ArticulationParameters)
	{
		OutArtParams.push_back(ArticulationParameter.ToOpenDIS());
	}
	detonationPDUOut.setArticulationParameters(OutArtParams);

	return detonationPDUOut;
}

TArray<uint8> UGRILL_DetonationPDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	ToOpenDIS().marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
