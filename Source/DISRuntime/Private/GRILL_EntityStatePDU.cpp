// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_EntityStatePDU.h"

UGRILL_EntityStatePDU::UGRILL_EntityStatePDU() : UGRILL_EntityInformationFamilyPDU()
{
	PDUStruct.PduType = EPDUType::EntityState;
}

void UGRILL_EntityStatePDU::SetupFromEntityStateUpdatePDU(UGRILL_EntityStateUpdatePDU* EntityStateUpdatePDUIn)
{
	//pdu common parameters
	PDUStruct = EntityStateUpdatePDUIn->PDUStruct;
	PDUStruct.PduType = EPDUType::EntityState;

	EntityStatePDUStruct.EntityID = EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityID;
	EntityStatePDUStruct.EntityLocationDouble = EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityLocationDouble;
	EntityStatePDUStruct.EntityLocation = EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityLocation;
	EntityStatePDUStruct.EntityOrientation = EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityOrientation;
	EntityStatePDUStruct.EntityLinearVelocity = EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityLinearVelocity;
	EntityStatePDUStruct.EntityAppearance = EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.EntityAppearance;
	EntityStatePDUStruct.ArticulationParameters = EntityStateUpdatePDUIn->EntityStateUpdatePDUStruct.ArticulationParameters;
}

void UGRILL_EntityStatePDU::SetupFromOpenDIS(DIS::EntityStatePdu* EntityStatePDUIn)
{
	UGRILL_EntityInformationFamilyPDU::SetupFromOpenDIS(EntityStatePDUIn);

	// Entity State PDU specifics
	//entity id
	EntityStatePDUStruct.EntityID = EntityStatePDUIn->getEntityID();

	//pure since unsupported in BP
	DIS::Vector3Double& position = EntityStatePDUIn->getEntityLocation();
	EntityStatePDUStruct.EntityLocationDouble[0] = position.getX();
	EntityStatePDUStruct.EntityLocationDouble[1] = position.getY();
	EntityStatePDUStruct.EntityLocationDouble[2] = position.getZ();

	//location
	EntityStatePDUStruct.EntityLocation[0] = position.getX();
	EntityStatePDUStruct.EntityLocation[1] = position.getY();
	EntityStatePDUStruct.EntityLocation[2] = position.getZ();

	//rotation
	DIS::Orientation& rotation = EntityStatePDUIn->getEntityOrientation();
	EntityStatePDUStruct.EntityOrientation.Yaw = rotation.getPsi();
	EntityStatePDUStruct.EntityOrientation.Roll = rotation.getPhi();
	EntityStatePDUStruct.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	EntityStatePDUStruct.EntityLinearVelocity[0] = EntityStatePDUIn->getEntityLinearVelocity().getX();
	EntityStatePDUStruct.EntityLinearVelocity[1] = EntityStatePDUIn->getEntityLinearVelocity().getY();
	EntityStatePDUStruct.EntityLinearVelocity[2] = EntityStatePDUIn->getEntityLinearVelocity().getZ();

	//Dead reckoning
	EntityStatePDUStruct.DeadReckoningParameters.DeadReckoningAlgorithm = EntityStatePDUIn->getDeadReckoningParameters().getDeadReckoningAlgorithm();
	EntityStatePDUStruct.DeadReckoningParameters.OtherParameters = TArray<uint8>(reinterpret_cast<uint8*>(EntityStatePDUIn->getDeadReckoningParameters().getOtherParameters()), 15);
	EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getX();
	EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getY();
	EntityStatePDUStruct.DeadReckoningParameters.EntityLinearAcceleration[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getZ();
	EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getX();
	EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getY();
	EntityStatePDUStruct.DeadReckoningParameters.EntityAngularVelocity[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getZ();

	//single vars
	EntityStatePDUStruct.ForceID = static_cast<EForceID>(EntityStatePDUIn->getForceId());
	EntityStatePDUStruct.Marking = FString(EntityStatePDUIn->getMarking().getCharacters());
	EntityStatePDUStruct.EntityAppearance = EntityStatePDUIn->getEntityAppearance();
	EntityStatePDUStruct.Capabilities = EntityStatePDUIn->getCapabilities();

	//Entity type
	EntityStatePDUStruct.EntityType = EntityStatePDUIn->getEntityType();

	//Alternative Entity type
	EntityStatePDUStruct.AlternativeEntityType = EntityStatePDUIn->getAlternativeEntityType();

	//Articulation Parameters
	for (int i = 0; i < EntityStatePDUIn->getNumberOfArticulationParameters(); i++)
	{
		DIS::ArticulationParameter tempArtParam = EntityStatePDUIn->getArticulationParameters()[i];
		FArticulationParameters newArtParam;
		newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
		newArtParam.ParameterType = tempArtParam.getParameterType();
		newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
		newArtParam.ParameterValue = tempArtParam.getParameterValue();
		newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();

		EntityStatePDUStruct.ArticulationParameters.Add(newArtParam);
	}
}

void UGRILL_EntityStatePDU::ToOpenDIS(DIS::EntityStatePdu& EntityStatePDUOut)
{
	UGRILL_EntityInformationFamilyPDU::ToOpenDIS(EntityStatePDUOut);

	// Specific PDU setup
	EntityStatePDUOut.setEntityID(EntityStatePDUStruct.EntityID.ToOpenDIS());
	EntityStatePDUOut.setForceId(static_cast<unsigned char>(EntityStatePDUStruct.ForceID));
	EntityStatePDUOut.setEntityType(EntityStatePDUStruct.EntityType.ToOpenDIS());
	EntityStatePDUOut.setAlternativeEntityType(EntityStatePDUStruct.AlternativeEntityType.ToOpenDIS());

	DIS::Vector3Float OutLinearVelocity;
	OutLinearVelocity.setX(EntityStatePDUStruct.EntityLinearVelocity.X);
	OutLinearVelocity.setY(EntityStatePDUStruct.EntityLinearVelocity.Y);
	OutLinearVelocity.setZ(EntityStatePDUStruct.EntityLinearVelocity.Z);
	EntityStatePDUOut.setEntityLinearVelocity(OutLinearVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(EntityStatePDUStruct.EntityLocationDouble[0]), EntityStatePDUStruct.EntityLocation.X) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStatePDUStruct.EntityLocationDouble[1]), EntityStatePDUStruct.EntityLocation.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStatePDUStruct.EntityLocationDouble[2]), EntityStatePDUStruct.EntityLocation.Z))
	{
		OutLocation.setX(EntityStatePDUStruct.EntityLocationDouble[0]);
		OutLocation.setY(EntityStatePDUStruct.EntityLocationDouble[1]);
		OutLocation.setZ(EntityStatePDUStruct.EntityLocationDouble[2]);
	}
	else
	{
		OutLocation.setX(EntityStatePDUStruct.EntityLocation.X);
		OutLocation.setY(EntityStatePDUStruct.EntityLocation.Y);
		OutLocation.setZ(EntityStatePDUStruct.EntityLocation.Z);
	}
	EntityStatePDUOut.setEntityLocation(OutLocation);

	DIS::Orientation OutOrientation;
	OutOrientation.setPsi(EntityStatePDUStruct.EntityOrientation.Yaw);
	OutOrientation.setTheta(EntityStatePDUStruct.EntityOrientation.Pitch);
	OutOrientation.setPhi(EntityStatePDUStruct.EntityOrientation.Roll);
	EntityStatePDUOut.setEntityOrientation(OutOrientation);

	EntityStatePDUOut.setEntityAppearance(EntityStatePDUStruct.EntityAppearance);
	EntityStatePDUOut.setDeadReckoningParameters(EntityStatePDUStruct.DeadReckoningParameters.ToOpenDIS());

	DIS::Marking OutMarking;
	OutMarking.setCharacterSet(1);
	OutMarking.setByStringCharacters(TCHAR_TO_ANSI(*EntityStatePDUStruct.Marking.Left(11)));
	EntityStatePDUOut.setMarking(OutMarking);

	EntityStatePDUOut.setCapabilities(EntityStatePDUStruct.Capabilities);

	std::vector<DIS::ArticulationParameter> OutArtParams;
	for (FArticulationParameters Param : EntityStatePDUStruct.ArticulationParameters)
	{
		OutArtParams.push_back(Param.ToOpenDIS());
	}
	EntityStatePDUOut.setArticulationParameters(OutArtParams);
}

TArray<uint8> UGRILL_EntityStatePDU::ToBytes()
{
	DIS::DataStream buffer(DIS::BIG);

	//marshal
	DIS::EntityStatePdu espdu;

	ToOpenDIS(espdu);
	espdu.marshal(buffer);

	TArray<uint8> byteArrayOut;
	byteArrayOut.Init(0, buffer.size());
	for (int i = 0; i < buffer.size(); i++) {
		byteArrayOut[i] = buffer[i];
	}
	buffer.clear();

	return byteArrayOut;
}
