// Fill out your copyright notice in the Description page of Project Settings.


#include "GRILL_EntityStatePDU.h"

UGRILL_EntityStatePDU::UGRILL_EntityStatePDU()
{
	EntityStatePduStruct.PduType = EPDUType::EntityState;
	EntityStatePduStruct.ForceID = EForceID::Other;
	EntityStatePduStruct.Marking = FString("");
	EntityStatePduStruct.EntityLocation = FVector(0, 0, 0);
	EntityStatePduStruct.EntityOrientation = FRotator(0, 0, 0);
	EntityStatePduStruct.EntityLocationDouble.Init(0, 3);
	EntityStatePduStruct.EntityLinearVelocity = FVector(0, 0, 0);
	EntityStatePduStruct.EntityAppearance = 0;
	EntityStatePduStruct.Capabilities = 0;
}

void UGRILL_EntityStatePDU::SetupFromEntityStateUpdatePDU(UGRILL_EntityStateUpdatePDU* EntityStateUpdatePDUIn)
{
	//pdu common parameters
	EntityStatePduStruct.ProtocolVersion = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.ProtocolVersion;
	EntityStatePduStruct.ExerciseID = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.ExerciseID;
	EntityStatePduStruct.PduType = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.PduType;
	EntityStatePduStruct.ProtocolFamily = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.ProtocolFamily;
	EntityStatePduStruct.Timestamp = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.Timestamp;
	EntityStatePduStruct.Length = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.Length;
	EntityStatePduStruct.Padding = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.Padding;

	EntityStatePduStruct.EntityID = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityID;
	EntityStatePduStruct.EntityLocationDouble = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityLocationDouble;
	EntityStatePduStruct.EntityLocation = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityLocation;
	EntityStatePduStruct.EntityOrientation = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityOrientation;
	EntityStatePduStruct.EntityLinearVelocity = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityLinearVelocity;
	EntityStatePduStruct.EntityAppearance = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.EntityAppearance;
	EntityStatePduStruct.ArticulationParameters = EntityStateUpdatePDUIn->EntityStateUpdatePduStruct.ArticulationParameters;
}

void UGRILL_EntityStatePDU::SetupFromOpenDIS(DIS::EntityStatePdu* EntityStatePDUIn)
{
	//pdu common parameters
	EntityStatePduStruct.ProtocolVersion = EntityStatePDUIn->getProtocolVersion();
	EntityStatePduStruct.ExerciseID = EntityStatePDUIn->getExerciseID();
	EntityStatePduStruct.PduType = static_cast<EPDUType>(EntityStatePDUIn->getPduType());
	EntityStatePduStruct.ProtocolFamily = EntityStatePDUIn->getProtocolFamily();
	EntityStatePduStruct.Timestamp = EntityStatePDUIn->getTimestamp();
	EntityStatePduStruct.Length = EntityStatePDUIn->getLength();
	EntityStatePduStruct.Padding = EntityStatePDUIn->getPadding();

	// Entity State PDU specifics
	//entity id
	EntityStatePduStruct.EntityID = EntityStatePDUIn->getEntityID();

	//pure since unsupported in BP
	DIS::Vector3Double& position = EntityStatePDUIn->getEntityLocation();
	EntityStatePduStruct.EntityLocationDouble[0] = position.getX();
	EntityStatePduStruct.EntityLocationDouble[1] = position.getY();
	EntityStatePduStruct.EntityLocationDouble[2] = position.getZ();

	//location
	EntityStatePduStruct.EntityLocation[0] = position.getX();
	EntityStatePduStruct.EntityLocation[1] = position.getY();
	EntityStatePduStruct.EntityLocation[2] = position.getZ();

	//rotation
	DIS::Orientation& rotation = EntityStatePDUIn->getEntityOrientation();
	EntityStatePduStruct.EntityOrientation.Yaw = rotation.getPsi();
	EntityStatePduStruct.EntityOrientation.Roll = rotation.getPhi();
	EntityStatePduStruct.EntityOrientation.Pitch = rotation.getTheta();

	//velocity (originally in float so this is fine)
	EntityStatePduStruct.EntityLinearVelocity[0] = EntityStatePDUIn->getEntityLinearVelocity().getX();
	EntityStatePduStruct.EntityLinearVelocity[1] = EntityStatePDUIn->getEntityLinearVelocity().getY();
	EntityStatePduStruct.EntityLinearVelocity[2] = EntityStatePDUIn->getEntityLinearVelocity().getZ();

	//Dead reckoning
	EntityStatePduStruct.DeadReckoningParameters.DeadReckoningAlgorithm = EntityStatePDUIn->getDeadReckoningParameters().getDeadReckoningAlgorithm();
	EntityStatePduStruct.DeadReckoningParameters.OtherParameters = TArray<uint8>(reinterpret_cast<uint8*>(EntityStatePDUIn->getDeadReckoningParameters().getOtherParameters()), 15);
	EntityStatePduStruct.DeadReckoningParameters.EntityLinearAcceleration[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getX();
	EntityStatePduStruct.DeadReckoningParameters.EntityLinearAcceleration[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getY();
	EntityStatePduStruct.DeadReckoningParameters.EntityLinearAcceleration[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getZ();
	EntityStatePduStruct.DeadReckoningParameters.EntityAngularVelocity[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getX();
	EntityStatePduStruct.DeadReckoningParameters.EntityAngularVelocity[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getY();
	EntityStatePduStruct.DeadReckoningParameters.EntityAngularVelocity[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getZ();

	//single vars
	EntityStatePduStruct.ForceID = static_cast<EForceID>(EntityStatePDUIn->getForceId());
	EntityStatePduStruct.Marking = FString(EntityStatePDUIn->getMarking().getCharacters());
	EntityStatePduStruct.EntityAppearance = EntityStatePDUIn->getEntityAppearance();
	EntityStatePduStruct.Capabilities = EntityStatePDUIn->getCapabilities();

	//Entity type
	EntityStatePduStruct.EntityType = EntityStatePDUIn->getEntityType();

	//Alternative Entity type
	EntityStatePduStruct.AlternativeEntityType = EntityStatePDUIn->getAlternativeEntityType();

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

		EntityStatePduStruct.ArticulationParameters.Add(newArtParam);
	}
}

DIS::EntityStatePdu UGRILL_EntityStatePDU::ToOpenDIS()
{
	DIS::EntityStatePdu entityStatePDUOut;

	// Common PDU setup
	entityStatePDUOut.setProtocolVersion(EntityStatePduStruct.ProtocolVersion);
	entityStatePDUOut.setExerciseID(EntityStatePduStruct.ExerciseID);
	entityStatePDUOut.setPduType(static_cast<unsigned char>(EntityStatePduStruct.PduType));
	entityStatePDUOut.setProtocolFamily(EntityStatePduStruct.ProtocolFamily);
	entityStatePDUOut.setTimestamp(EntityStatePduStruct.Timestamp);
	entityStatePDUOut.setLength(EntityStatePduStruct.Length);
	entityStatePDUOut.setPadding(EntityStatePduStruct.Padding);

	// Specific PDU setup
	entityStatePDUOut.setEntityID(EntityStatePduStruct.EntityID.ToOpenDIS());
	entityStatePDUOut.setForceId(static_cast<unsigned char>(EntityStatePduStruct.ForceID));
	entityStatePDUOut.setEntityType(EntityStatePduStruct.EntityType.ToOpenDIS());
	entityStatePDUOut.setAlternativeEntityType(EntityStatePduStruct.AlternativeEntityType.ToOpenDIS());

	DIS::Vector3Float OutLinearVelocity;
	OutLinearVelocity.setX(EntityStatePduStruct.EntityLinearVelocity.X);
	OutLinearVelocity.setY(EntityStatePduStruct.EntityLinearVelocity.Y);
	OutLinearVelocity.setZ(EntityStatePduStruct.EntityLinearVelocity.Z);
	entityStatePDUOut.setEntityLinearVelocity(OutLinearVelocity);

	DIS::Vector3Double OutLocation;
	if (FMath::IsNearlyEqual(static_cast<float>(EntityStatePduStruct.EntityLocationDouble[0]), EntityStatePduStruct.EntityLocation.X) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStatePduStruct.EntityLocationDouble[1]), EntityStatePduStruct.EntityLocation.Y) &&
		FMath::IsNearlyEqual(static_cast<float>(EntityStatePduStruct.EntityLocationDouble[2]), EntityStatePduStruct.EntityLocation.Z))
	{
		OutLocation.setX(EntityStatePduStruct.EntityLocationDouble[0]);
		OutLocation.setY(EntityStatePduStruct.EntityLocationDouble[1]);
		OutLocation.setZ(EntityStatePduStruct.EntityLocationDouble[2]);
	}
	else
	{
		OutLocation.setX(EntityStatePduStruct.EntityLocation.X);
		OutLocation.setY(EntityStatePduStruct.EntityLocation.Y);
		OutLocation.setZ(EntityStatePduStruct.EntityLocation.Z);
	}
	entityStatePDUOut.setEntityLocation(OutLocation);

	DIS::Orientation OutOrientation;
	OutOrientation.setPsi(EntityStatePduStruct.EntityOrientation.Yaw);
	OutOrientation.setTheta(EntityStatePduStruct.EntityOrientation.Pitch);
	OutOrientation.setPhi(EntityStatePduStruct.EntityOrientation.Roll);
	entityStatePDUOut.setEntityOrientation(OutOrientation);

	entityStatePDUOut.setEntityAppearance(EntityStatePduStruct.EntityAppearance);
	entityStatePDUOut.setDeadReckoningParameters(EntityStatePduStruct.DeadReckoningParameters.ToOpenDIS());

	DIS::Marking OutMarking;
	OutMarking.setCharacterSet(1);
	OutMarking.setByStringCharacters(TCHAR_TO_ANSI(*EntityStatePduStruct.Marking.Left(11)));
	entityStatePDUOut.setMarking(OutMarking);

	entityStatePDUOut.setCapabilities(EntityStatePduStruct.Capabilities);

	std::vector<DIS::ArticulationParameter> OutArtParams;
	for (FArticulationParameters Param : EntityStatePduStruct.ArticulationParameters)
	{
		OutArtParams.push_back(Param.ToOpenDIS());
	}
	entityStatePDUOut.setArticulationParameters(OutArtParams);

	return entityStatePDUOut;
}

TArray<uint8> UGRILL_EntityStatePDU::ToBytes()
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
