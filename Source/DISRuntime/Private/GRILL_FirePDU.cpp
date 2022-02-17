// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_FirePDU.h"

UGRILL_FirePDU::UGRILL_FirePDU() : UGRILL_WarfareFamilyPDU()
{
	PDUStruct.PduType = EPDUType::Fire;
}

void UGRILL_FirePDU::SetupFromOpenDIS(DIS::FirePdu* FirePDUIn)
{
	UGRILL_WarfareFamilyPDU::SetupFromOpenDIS(FirePDUIn);

	// Fire PDU specifics
	//single vars
	FirePDUStruct.FireMissionIndex = FirePDUIn->getFireMissionIndex();
	FirePDUStruct.Range = FirePDUIn->getRange();

	//MunitionEntityID
	FirePDUStruct.MunitionEntityID.Site = FirePDUIn->getMunitionID().getSite();
	FirePDUStruct.MunitionEntityID.Application = FirePDUIn->getMunitionID().getApplication();
	FirePDUStruct.MunitionEntityID.Entity = FirePDUIn->getMunitionID().getEntity();

	//velocity
	FirePDUStruct.Velocity[0] = FirePDUIn->getVelocity().getX();
	FirePDUStruct.Velocity[1] = FirePDUIn->getVelocity().getY();
	FirePDUStruct.Velocity[2] = FirePDUIn->getVelocity().getZ();

	//location
	FirePDUStruct.Location[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	FirePDUStruct.Location[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	FirePDUStruct.Location[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//locationDouble
	FirePDUStruct.LocationDouble[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
	FirePDUStruct.LocationDouble[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
	FirePDUStruct.LocationDouble[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

	//event id
	FirePDUStruct.EventID = FirePDUIn->getEventID();

	//burst descriptor
	FirePDUStruct.BurstDescriptor.Warhead = FirePDUIn->getBurstDescriptor().getWarhead();
	FirePDUStruct.BurstDescriptor.Fuse = FirePDUIn->getBurstDescriptor().getFuse();
	FirePDUStruct.BurstDescriptor.Rate = FirePDUIn->getBurstDescriptor().getRate();
	FirePDUStruct.BurstDescriptor.Quantity = FirePDUIn->getBurstDescriptor().getQuantity();
	FirePDUStruct.BurstDescriptor.EntityType = FirePDUIn->getBurstDescriptor().getMunition();
}

void UGRILL_FirePDU::ToOpenDIS(DIS::FirePdu& FirePDUOut)
{
	UGRILL_WarfareFamilyPDU::ToOpenDIS(FirePDUOut);

	//Inherited PDU setup
	FirePDUOut.setFiringEntityID(WarfareFamilyPDUStruct.FiringEntityID.ToOpenDIS());
	FirePDUOut.setTargetEntityID(WarfareFamilyPDUStruct.TargetEntityID.ToOpenDIS());

	// Specific PDU setup
	FirePDUOut.setMunitionID(FirePDUStruct.MunitionEntityID.ToOpenDIS());
	FirePDUOut.setFireMissionIndex(FirePDUStruct.FireMissionIndex);
	FirePDUOut.setRange(FirePDUStruct.Range);

	DIS::Vector3Float OutVelocity;
	OutVelocity.setX(FirePDUStruct.Velocity.X);
	OutVelocity.setY(FirePDUStruct.Velocity.Y);
	OutVelocity.setZ(FirePDUStruct.Velocity.Z);
	FirePDUOut.setVelocity(OutVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(FirePDUStruct.LocationDouble[0]), FirePDUStruct.Location.X) &&
		FMath::IsNearlyEqual(static_cast<float>(FirePDUStruct.LocationDouble[1]), FirePDUStruct.Location.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(FirePDUStruct.LocationDouble[2]), FirePDUStruct.Location.Z))
	{
		OutLocation.setX(FirePDUStruct.LocationDouble[0]);
		OutLocation.setY(FirePDUStruct.LocationDouble[1]);
		OutLocation.setZ(FirePDUStruct.LocationDouble[2]);
	}
	else
	{
		OutLocation.setX(FirePDUStruct.Location.X);
		OutLocation.setY(FirePDUStruct.Location.Y);
		OutLocation.setZ(FirePDUStruct.Location.Z);
	}
	FirePDUOut.setLocationInWorldCoordinates(OutLocation);

	FirePDUOut.setEventID(FirePDUStruct.EventID.ToOpenDIS());
	FirePDUOut.setBurstDescriptor(FirePDUStruct.BurstDescriptor.ToOpenDIS());
}

TArray<uint8> UGRILL_FirePDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::FirePdu firePDU;

	ToOpenDIS(firePDU);
	firePDU.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
