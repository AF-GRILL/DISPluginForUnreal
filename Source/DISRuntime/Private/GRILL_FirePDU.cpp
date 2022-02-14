// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_FirePDU.h"

UGRILL_FirePDU::UGRILL_FirePDU()
{
	FirePduStruct.PduType = EPDUType::Fire;
	FirePduStruct.FireMissionIndex = 0;
	FirePduStruct.Range = 0.0;
	FirePduStruct.LocationDouble.Init(0, 3);
	FirePduStruct.Location = FVector(0, 0, 0);
	FirePduStruct.Velocity = FVector(0, 0, 0);
}

void UGRILL_FirePDU::SetupFromOpenDIS(DIS::FirePdu* FirePDUIn)
{
	//pdu common parameters
	FirePduStruct.ProtocolVersion = FirePDUIn->getProtocolVersion();
	FirePduStruct.ExerciseID = FirePDUIn->getExerciseID();
	FirePduStruct.PduType = static_cast<EPDUType>(FirePDUIn->getPduType());
	FirePduStruct.ProtocolFamily = FirePDUIn->getProtocolFamily();
	FirePduStruct.Timestamp = FirePDUIn->getTimestamp();
	FirePduStruct.Length = FirePDUIn->getLength();
	FirePduStruct.Padding = FirePDUIn->getPadding();

	// WarfareFamilyPdu specific parameters
	FirePduStruct.FiringEntityID = FirePDUIn->getFiringEntityID();
	FirePduStruct.TargetEntityID = FirePDUIn->getTargetEntityID();

	// Fire PDU specifics
	//single vars
	FirePduStruct.FireMissionIndex = FirePDUIn->getFireMissionIndex();
	FirePduStruct.Range = FirePDUIn->getRange();

	//MunitionEntityID
	FirePduStruct.MunitionEntityID.Site = FirePDUIn->getMunitionID().getSite();
	FirePduStruct.MunitionEntityID.Application = FirePDUIn->getMunitionID().getApplication();
	FirePduStruct.MunitionEntityID.Entity = FirePDUIn->getMunitionID().getEntity();

	//velocity
	FirePduStruct.Velocity[0] = FirePDUIn->getVelocity().getX();
	FirePduStruct.Velocity[1] = FirePDUIn->getVelocity().getY();
	FirePduStruct.Velocity[2] = FirePDUIn->getVelocity().getZ();

	//location
	FirePduStruct.Location[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	FirePduStruct.Location[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	FirePduStruct.Location[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	FirePduStruct.LocationDouble[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	FirePduStruct.LocationDouble[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	FirePduStruct.LocationDouble[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//event id
	FirePduStruct.EventID = FirePDUIn->getEventID();

	//burst descriptor
	FirePduStruct.BurstDescriptor.Warhead = FirePDUIn->getBurstDescriptor().getWarhead();
	FirePduStruct.BurstDescriptor.Fuse = FirePDUIn->getBurstDescriptor().getFuse();
	FirePduStruct.BurstDescriptor.Rate = FirePDUIn->getBurstDescriptor().getRate();
	FirePduStruct.BurstDescriptor.Quantity = FirePDUIn->getBurstDescriptor().getQuantity();
	FirePduStruct.BurstDescriptor.EntityType = FirePDUIn->getBurstDescriptor().getMunition();
}

DIS::FirePdu UGRILL_FirePDU::ToOpenDIS()
{
	DIS::FirePdu firePDUOut;

	// Common PDU setup
	firePDUOut.setProtocolVersion(FirePduStruct.ProtocolVersion);
	firePDUOut.setExerciseID(FirePduStruct.ExerciseID);
	firePDUOut.setPduType(static_cast<unsigned char>(FirePduStruct.PduType));
	firePDUOut.setProtocolFamily(FirePduStruct.ProtocolFamily);
	firePDUOut.setTimestamp(FirePduStruct.Timestamp);
	firePDUOut.setLength(FirePduStruct.Length);
	firePDUOut.setPadding(FirePduStruct.Padding);

	// Warfare Family PDU setup
	firePDUOut.setFiringEntityID(FirePduStruct.FiringEntityID.ToOpenDIS());
	firePDUOut.setTargetEntityID(FirePduStruct.TargetEntityID.ToOpenDIS());

	// Specific PDU setup
	firePDUOut.setMunitionID(FirePduStruct.MunitionEntityID.ToOpenDIS());
	firePDUOut.setFireMissionIndex(FirePduStruct.FireMissionIndex);
	firePDUOut.setRange(FirePduStruct.Range);

	DIS::Vector3Float OutVelocity;
	OutVelocity.setX(FirePduStruct.Velocity.X);
	OutVelocity.setY(FirePduStruct.Velocity.Y);
	OutVelocity.setZ(FirePduStruct.Velocity.Z);
	firePDUOut.setVelocity(OutVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(FirePduStruct.LocationDouble[0]), FirePduStruct.Location.X) &&
		FMath::IsNearlyEqual(static_cast<float>(FirePduStruct.LocationDouble[1]), FirePduStruct.Location.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(FirePduStruct.LocationDouble[2]), FirePduStruct.Location.Z))
	{
		OutLocation.setX(FirePduStruct.LocationDouble[0]);
		OutLocation.setY(FirePduStruct.LocationDouble[1]);
		OutLocation.setZ(FirePduStruct.LocationDouble[2]);
	}
	else
	{
		OutLocation.setX(FirePduStruct.Location.X);
		OutLocation.setY(FirePduStruct.Location.Y);
		OutLocation.setZ(FirePduStruct.Location.Z);
	}
	firePDUOut.setLocationInWorldCoordinates(OutLocation);

	firePDUOut.setEventID(FirePduStruct.EventID.ToOpenDIS());
	firePDUOut.setBurstDescriptor(FirePduStruct.BurstDescriptor.ToOpenDIS());
	firePDUOut.setFiringEntityID(FirePduStruct.FiringEntityID.ToOpenDIS());
	firePDUOut.setTargetEntityID(FirePduStruct.TargetEntityID.ToOpenDIS());

	return firePDUOut;
}

TArray<uint8> UGRILL_FirePDU::ToBytes()
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
