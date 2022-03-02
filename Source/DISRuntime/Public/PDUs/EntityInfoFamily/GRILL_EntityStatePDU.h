// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/EntityStatePdu.h> 
#include "PDUs/EntityInfoFamily/GRILL_EntityStateUpdatePDU.h"
#include "GRILL_EntityStatePDU.generated.h"

USTRUCT(BlueprintType)
struct FEntityStatePDU : public FEntityInformationFamilyPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID EntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EForceID ForceID;
	/** The location of the entity in ECEF - double (64-bit) precision */
	UPROPERTY()
		TArray<double> EntityLocationDouble;
	/** The location of the entity in ECEF - floating point (32-bit) precision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLocation;
	/** The orientation of the entity in Psi (Yaw), Theta (Pitch), Phi (Roll) - in radians */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator EntityOrientation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Marking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EntityLinearVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDeadReckoningParameters DeadReckoningParameters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityType EntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EntityAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Capabilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityType AlternativeEntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FArticulationParameters> ArticulationParameters;

	FEntityStatePDU() : FEntityInformationFamilyPDU()
	{
		PduType = EPDUType::EntityState;

		ForceID = EForceID::Other;
		Marking = FString("");
		EntityLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLocationDouble.Init(0, 3);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		Capabilities = 0;
		ArticulationParameters = TArray<FArticulationParameters>();
	}

	virtual ~FEntityStatePDU() {}

	void SetupFromOpenDIS(DIS::EntityStatePdu* EntityStatePDUIn)
	{
		FEntityInformationFamilyPDU::SetupFromOpenDIS(EntityStatePDUIn);

		// Entity State PDU specifics
		//entity id
		EntityID = EntityStatePDUIn->getEntityID();

		//pure since unsupported in BP
		DIS::Vector3Double& position = EntityStatePDUIn->getEntityLocation();
		EntityLocationDouble[0] = position.getX();
		EntityLocationDouble[1] = position.getY();
		EntityLocationDouble[2] = position.getZ();

		//location
		EntityLocation[0] = position.getX();
		EntityLocation[1] = position.getY();
		EntityLocation[2] = position.getZ();

		//rotation
		DIS::Orientation& rotation = EntityStatePDUIn->getEntityOrientation();
		EntityOrientation.Yaw = rotation.getPsi();
		EntityOrientation.Roll = rotation.getPhi();
		EntityOrientation.Pitch = rotation.getTheta();

		//velocity (originally in float so this is fine)
		EntityLinearVelocity[0] = EntityStatePDUIn->getEntityLinearVelocity().getX();
		EntityLinearVelocity[1] = EntityStatePDUIn->getEntityLinearVelocity().getY();
		EntityLinearVelocity[2] = EntityStatePDUIn->getEntityLinearVelocity().getZ();

		//Dead reckoning
		DeadReckoningParameters.DeadReckoningAlgorithm = EntityStatePDUIn->getDeadReckoningParameters().getDeadReckoningAlgorithm();
		DeadReckoningParameters.OtherParameters = TArray<uint8>(reinterpret_cast<uint8*>(EntityStatePDUIn->getDeadReckoningParameters().getOtherParameters()), 15);
		DeadReckoningParameters.EntityLinearAcceleration[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getX();
		DeadReckoningParameters.EntityLinearAcceleration[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getY();
		DeadReckoningParameters.EntityLinearAcceleration[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityLinearAcceleration().getZ();
		DeadReckoningParameters.EntityAngularVelocity[0] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getX();
		DeadReckoningParameters.EntityAngularVelocity[1] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getY();
		DeadReckoningParameters.EntityAngularVelocity[2] = EntityStatePDUIn->getDeadReckoningParameters().getEntityAngularVelocity().getZ();

		//single vars
		ForceID = static_cast<EForceID>(EntityStatePDUIn->getForceId());
		Marking = FString(EntityStatePDUIn->getMarking().getCharacters());
		EntityAppearance = EntityStatePDUIn->getEntityAppearance();
		Capabilities = EntityStatePDUIn->getCapabilities();

		//Entity type
		EntityType = EntityStatePDUIn->getEntityType();

		//Alternative Entity type
		AlternativeEntityType = EntityStatePDUIn->getAlternativeEntityType();

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

			ArticulationParameters.Add(newArtParam);
		}
	}

	void ToOpenDIS(DIS::EntityStatePdu& EntityStatePDUOut)
	{
		FEntityInformationFamilyPDU::ToOpenDIS(EntityStatePDUOut);

		// Specific PDU setup
		EntityStatePDUOut.setEntityID(EntityID.ToOpenDIS());
		EntityStatePDUOut.setForceId(static_cast<unsigned char>(ForceID));
		EntityStatePDUOut.setEntityType(EntityType.ToOpenDIS());
		EntityStatePDUOut.setAlternativeEntityType(AlternativeEntityType.ToOpenDIS());

		DIS::Vector3Float OutLinearVelocity;
		OutLinearVelocity.setX(EntityLinearVelocity.X);
		OutLinearVelocity.setY(EntityLinearVelocity.Y);
		OutLinearVelocity.setZ(EntityLinearVelocity.Z);
		EntityStatePDUOut.setEntityLinearVelocity(OutLinearVelocity);

		DIS::Vector3Double OutLocation;
		if (FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[0]), EntityLocation.X) &&
			FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[1]), EntityLocation.Y) &&
			FMath::IsNearlyEqual(static_cast<float>(EntityLocationDouble[2]), EntityLocation.Z))
		{
			OutLocation.setX(EntityLocationDouble[0]);
			OutLocation.setY(EntityLocationDouble[1]);
			OutLocation.setZ(EntityLocationDouble[2]);
		}
		else
		{
			OutLocation.setX(EntityLocation.X);
			OutLocation.setY(EntityLocation.Y);
			OutLocation.setZ(EntityLocation.Z);
		}
		EntityStatePDUOut.setEntityLocation(OutLocation);

		DIS::Orientation OutOrientation;
		OutOrientation.setPsi(EntityOrientation.Yaw);
		OutOrientation.setTheta(EntityOrientation.Pitch);
		OutOrientation.setPhi(EntityOrientation.Roll);
		EntityStatePDUOut.setEntityOrientation(OutOrientation);

		EntityStatePDUOut.setEntityAppearance(EntityAppearance);
		EntityStatePDUOut.setDeadReckoningParameters(DeadReckoningParameters.ToOpenDIS());

		DIS::Marking OutMarking;
		OutMarking.setCharacterSet(1);
		OutMarking.setByStringCharacters(TCHAR_TO_ANSI(*Marking.Left(11)));
		EntityStatePDUOut.setMarking(OutMarking);

		EntityStatePDUOut.setCapabilities(Capabilities);

		std::vector<DIS::ArticulationParameter> OutArtParams;
		for (FArticulationParameters Param : ArticulationParameters)
		{
			OutArtParams.push_back(Param.ToOpenDIS());
		}
		EntityStatePDUOut.setArticulationParameters(OutArtParams);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::EntityStatePdu espdu;

		ToOpenDIS(espdu);
		espdu.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}

	void operator = (FEntityStateUpdatePDU EntityStateUpdatePDUIn)
	{
		//pdu common parameters
		ProtocolVersion = EntityStateUpdatePDUIn.ProtocolVersion;
		ExerciseID = EntityStateUpdatePDUIn.ExerciseID;
		ProtocolFamily = EntityStateUpdatePDUIn.ProtocolFamily;
		Timestamp = EntityStateUpdatePDUIn.Timestamp;
		Length = EntityStateUpdatePDUIn.Length;
		Padding = EntityStateUpdatePDUIn.Padding;

		PduType = EPDUType::EntityState;

		//Entity State Update common parameters
		EntityID = EntityStateUpdatePDUIn.EntityID;
		EntityLocationDouble = EntityStateUpdatePDUIn.EntityLocationDouble;
		EntityLocation = EntityStateUpdatePDUIn.EntityLocation;
		EntityOrientation = EntityStateUpdatePDUIn.EntityOrientation;
		EntityLinearVelocity = EntityStateUpdatePDUIn.EntityLinearVelocity;
		EntityAppearance = EntityStateUpdatePDUIn.EntityAppearance;
		ArticulationParameters = EntityStateUpdatePDUIn.ArticulationParameters;
	}
};