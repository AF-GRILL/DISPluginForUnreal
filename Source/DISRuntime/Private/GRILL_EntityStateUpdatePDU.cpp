// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_EntityStateUpdatePDU.h"

UGRILL_EntityStateUpdatePDU::UGRILL_EntityStateUpdatePDU() : UGRILL_EntityInformationFamilyPDU()
{
	PDUStruct.PduType = EPDUType::EntityStateUpdate;
}

void UGRILL_EntityStateUpdatePDU::SetupFromOpenDIS(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn)
{
	UGRILL_EntityInformationFamilyPDU::SetupFromOpenDIS(EntityStateUpdatePDUIn);

	//Entity State Update specifics
	//entity id
	EntityStateUpdatePDUStruct.EntityID = EntityStateUpdatePDUIn->getEntityID();

	//pure since unsupported in BP
	DIS::Vector3Double& position = EntityStateUpdatePDUIn->getEntityLocation();
	EntityStateUpdatePDUStruct.EntityLocationDouble[0] = position.getX();
	EntityStateUpdatePDUStruct.EntityLocationDouble[1] = position.getY();
	EntityStateUpdatePDUStruct.EntityLocationDouble[2] = position.getZ();

	//location
	EntityStateUpdatePDUStruct.EntityLocation[0] = position.getX();
	EntityStateUpdatePDUStruct.EntityLocation[1] = position.getY();
	EntityStateUpdatePDUStruct.EntityLocation[2] = position.getZ();

	//rotation
	DIS::Orientation& rotation = EntityStateUpdatePDUIn->getEntityOrientation();
	EntityStateUpdatePDUStruct.EntityOrientation.Yaw = rotation.getPsi();
	EntityStateUpdatePDUStruct.EntityOrientation.Roll = rotation.getPhi();
	EntityStateUpdatePDUStruct.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	EntityStateUpdatePDUStruct.EntityLinearVelocity[0] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getX();
	EntityStateUpdatePDUStruct.EntityLinearVelocity[1] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getY();
	EntityStateUpdatePDUStruct.EntityLinearVelocity[2] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getZ();

	//Single Vars
	EntityStateUpdatePDUStruct.Padding1 = EntityStateUpdatePDUIn->getPadding1();
	EntityStateUpdatePDUStruct.EntityAppearance = EntityStateUpdatePDUIn->getEntityAppearance();

	//Articulation Parameters
	for (int i = 0; i < EntityStateUpdatePDUIn->getNumberOfArticulationParameters(); i++)
	{
		DIS::ArticulationParameter tempArtParam = EntityStateUpdatePDUIn->getArticulationParameters()[i];
		FArticulationParameters newArtParam;
		newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
		newArtParam.ParameterType = tempArtParam.getParameterType();
		newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
		newArtParam.ParameterValue = tempArtParam.getParameterValue();
		newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();

		EntityStateUpdatePDUStruct.ArticulationParameters.Add(newArtParam);
	}
}

void UGRILL_EntityStateUpdatePDU::ToOpenDIS(DIS::EntityStateUpdatePdu& EntityStateUpdatePDUOut)
{
	UGRILL_EntityInformationFamilyPDU::ToOpenDIS(EntityStateUpdatePDUOut);

	// Specific PDU setup
	EntityStateUpdatePDUOut.setEntityID(EntityStateUpdatePDUStruct.EntityID.ToOpenDIS());
	EntityStateUpdatePDUOut.setPadding1(EntityStateUpdatePDUStruct.Padding1);

	DIS::Vector3Float OutLinearVelocity;
	OutLinearVelocity.setX(EntityStateUpdatePDUStruct.EntityLinearVelocity.X);
	OutLinearVelocity.setY(EntityStateUpdatePDUStruct.EntityLinearVelocity.Y);
	OutLinearVelocity.setZ(EntityStateUpdatePDUStruct.EntityLinearVelocity.Z);
	EntityStateUpdatePDUOut.setEntityLinearVelocity(OutLinearVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(EntityStateUpdatePDUStruct.EntityLocationDouble[0]), EntityStateUpdatePDUStruct.EntityLocation.X) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStateUpdatePDUStruct.EntityLocationDouble[1]), EntityStateUpdatePDUStruct.EntityLocation.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStateUpdatePDUStruct.EntityLocationDouble[2]), EntityStateUpdatePDUStruct.EntityLocation.Z))
	{
		OutLocation.setX(EntityStateUpdatePDUStruct.EntityLocationDouble[0]);
		OutLocation.setY(EntityStateUpdatePDUStruct.EntityLocationDouble[1]);
		OutLocation.setZ(EntityStateUpdatePDUStruct.EntityLocationDouble[2]);
	}
	else
	{
		OutLocation.setX(EntityStateUpdatePDUStruct.EntityLocation.X);
		OutLocation.setY(EntityStateUpdatePDUStruct.EntityLocation.Y);
		OutLocation.setZ(EntityStateUpdatePDUStruct.EntityLocation.Z);
	}
	EntityStateUpdatePDUOut.setEntityLocation(OutLocation);

	DIS::Orientation OutOrientation;
	OutOrientation.setPsi(EntityStateUpdatePDUStruct.EntityOrientation.Yaw);
	OutOrientation.setTheta(EntityStateUpdatePDUStruct.EntityOrientation.Pitch);
	OutOrientation.setPhi(EntityStateUpdatePDUStruct.EntityOrientation.Roll);
	EntityStateUpdatePDUOut.setEntityOrientation(OutOrientation);

	EntityStateUpdatePDUOut.setEntityAppearance(EntityStateUpdatePDUStruct.EntityAppearance);

	std::vector<DIS::ArticulationParameter> OutArtParams;
	for (FArticulationParameters ArticulationParameter : EntityStateUpdatePDUStruct.ArticulationParameters)
	{
		OutArtParams.push_back(ArticulationParameter.ToOpenDIS());
	}
	EntityStateUpdatePDUOut.setArticulationParameters(OutArtParams);
}

TArray<uint8> UGRILL_EntityStateUpdatePDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::EntityStateUpdatePdu esupdu;

	ToOpenDIS(esupdu);
	esupdu.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
