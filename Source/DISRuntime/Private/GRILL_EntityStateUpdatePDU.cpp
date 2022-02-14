// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_EntityStateUpdatePDU.h"

UGRILL_EntityStateUpdatePDU::UGRILL_EntityStateUpdatePDU()
{
	EntityStateUpdatePduStruct.PduType = EPDUType::EntityStateUpdate;
	EntityStateUpdatePduStruct.EntityLocation = FVector(0, 0, 0);
	EntityStateUpdatePduStruct.EntityOrientation = FRotator(0, 0, 0);
	EntityStateUpdatePduStruct.EntityLocationDouble.Init(0, 3);
	EntityStateUpdatePduStruct.EntityLinearVelocity = FVector(0, 0, 0);
	EntityStateUpdatePduStruct.EntityAppearance = 0;
	EntityStateUpdatePduStruct.Padding = 0;
	EntityStateUpdatePduStruct.Padding1 = 0;
}

void UGRILL_EntityStateUpdatePDU::SetupFromOpenDIS(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn)
{
	//pdu common parameters
	EntityStateUpdatePduStruct.ProtocolVersion = EntityStateUpdatePDUIn->getProtocolVersion();
	EntityStateUpdatePduStruct.ExerciseID = EntityStateUpdatePDUIn->getExerciseID();
	EntityStateUpdatePduStruct.PduType = static_cast<EPDUType>(EntityStateUpdatePDUIn->getPduType());
	EntityStateUpdatePduStruct.ProtocolFamily = EntityStateUpdatePDUIn->getProtocolFamily();
	EntityStateUpdatePduStruct.Timestamp = EntityStateUpdatePDUIn->getTimestamp();
	EntityStateUpdatePduStruct.Length = EntityStateUpdatePDUIn->getLength();
	EntityStateUpdatePduStruct.Padding = EntityStateUpdatePDUIn->getPadding();

	//Entity State Update specifics
	//entity id
	EntityStateUpdatePduStruct.EntityID = EntityStateUpdatePDUIn->getEntityID();

	//pure since unsupported in BP
	DIS::Vector3Double& position = EntityStateUpdatePDUIn->getEntityLocation();
	EntityStateUpdatePduStruct.EntityLocationDouble[0] = position.getX();
	EntityStateUpdatePduStruct.EntityLocationDouble[1] = position.getY();
	EntityStateUpdatePduStruct.EntityLocationDouble[2] = position.getZ();

	//location
	EntityStateUpdatePduStruct.EntityLocation[0] = position.getX();
	EntityStateUpdatePduStruct.EntityLocation[1] = position.getY();
	EntityStateUpdatePduStruct.EntityLocation[2] = position.getZ();

	//rotation
	DIS::Orientation& rotation = EntityStateUpdatePDUIn->getEntityOrientation();
	EntityStateUpdatePduStruct.EntityOrientation.Yaw = rotation.getPsi();
	EntityStateUpdatePduStruct.EntityOrientation.Roll = rotation.getPhi();
	EntityStateUpdatePduStruct.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	EntityStateUpdatePduStruct.EntityLinearVelocity[0] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getX();
	EntityStateUpdatePduStruct.EntityLinearVelocity[1] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getY();
	EntityStateUpdatePduStruct.EntityLinearVelocity[2] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getZ();

	//Single Vars
	EntityStateUpdatePduStruct.Padding1 = EntityStateUpdatePDUIn->getPadding1();
	EntityStateUpdatePduStruct.EntityAppearance = EntityStateUpdatePDUIn->getEntityAppearance();

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

		EntityStateUpdatePduStruct.ArticulationParameters.Add(newArtParam);
	}
}

DIS::EntityStateUpdatePdu UGRILL_EntityStateUpdatePDU::ToOpenDIS()
{
	DIS::EntityStateUpdatePdu entityStateUpdatePDUOut;

	// Common PDU setup
	entityStateUpdatePDUOut.setProtocolVersion(EntityStateUpdatePduStruct.ProtocolVersion);
	entityStateUpdatePDUOut.setExerciseID(EntityStateUpdatePduStruct.ExerciseID);
	entityStateUpdatePDUOut.setPduType(static_cast<unsigned char>(EntityStateUpdatePduStruct.PduType));
	entityStateUpdatePDUOut.setProtocolFamily(EntityStateUpdatePduStruct.ProtocolFamily);
	entityStateUpdatePDUOut.setTimestamp(EntityStateUpdatePduStruct.Timestamp);
	entityStateUpdatePDUOut.setLength(EntityStateUpdatePduStruct.Length);
	entityStateUpdatePDUOut.setPadding(EntityStateUpdatePduStruct.Padding);

	// Specific PDU setup
	entityStateUpdatePDUOut.setEntityID(EntityStateUpdatePduStruct.EntityID.ToOpenDIS());
	entityStateUpdatePDUOut.setPadding1(EntityStateUpdatePduStruct.Padding1);

	DIS::Vector3Float OutLinearVelocity;
	OutLinearVelocity.setX(EntityStateUpdatePduStruct.EntityLinearVelocity.X);
	OutLinearVelocity.setY(EntityStateUpdatePduStruct.EntityLinearVelocity.Y);
	OutLinearVelocity.setZ(EntityStateUpdatePduStruct.EntityLinearVelocity.Z);
	entityStateUpdatePDUOut.setEntityLinearVelocity(OutLinearVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(EntityStateUpdatePduStruct.EntityLocationDouble[0]), EntityStateUpdatePduStruct.EntityLocation.X) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStateUpdatePduStruct.EntityLocationDouble[1]), EntityStateUpdatePduStruct.EntityLocation.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStateUpdatePduStruct.EntityLocationDouble[2]), EntityStateUpdatePduStruct.EntityLocation.Z))
	{
		OutLocation.setX(EntityStateUpdatePduStruct.EntityLocationDouble[0]);
		OutLocation.setY(EntityStateUpdatePduStruct.EntityLocationDouble[1]);
		OutLocation.setZ(EntityStateUpdatePduStruct.EntityLocationDouble[2]);
	}
	else
	{
		OutLocation.setX(EntityStateUpdatePduStruct.EntityLocation.X);
		OutLocation.setY(EntityStateUpdatePduStruct.EntityLocation.Y);
		OutLocation.setZ(EntityStateUpdatePduStruct.EntityLocation.Z);
	}
	entityStateUpdatePDUOut.setEntityLocation(OutLocation);

	DIS::Orientation OutOrientation;
	OutOrientation.setPsi(EntityStateUpdatePduStruct.EntityOrientation.Yaw);
	OutOrientation.setTheta(EntityStateUpdatePduStruct.EntityOrientation.Pitch);
	OutOrientation.setPhi(EntityStateUpdatePduStruct.EntityOrientation.Roll);
	entityStateUpdatePDUOut.setEntityOrientation(OutOrientation);

	entityStateUpdatePDUOut.setEntityAppearance(EntityStateUpdatePduStruct.EntityAppearance);

	std::vector<DIS::ArticulationParameter> OutArtParams;
	for (FArticulationParameters ArticulationParameter : EntityStateUpdatePduStruct.ArticulationParameters)
	{
		OutArtParams.push_back(ArticulationParameter.ToOpenDIS());
	}
	entityStateUpdatePDUOut.setArticulationParameters(OutArtParams);

	return entityStateUpdatePDUOut;
}

TArray<uint8> UGRILL_EntityStateUpdatePDU::ToBytes()
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
