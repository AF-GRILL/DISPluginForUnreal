// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

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

	/** The site, application, and unique identifier for this entity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FEntityID EntityID;
	/** Enumeration to distinguish different teams or sides in a scenario. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		EForceID ForceID;
	/** The location of the entity in ECEF - double (64-bit) precision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FVector EcefLocation;
	/** The orientation of the entity in Psi (Yaw), Theta (Pitch), Phi (Roll) - in radians */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FRotator EntityOrientation;
	/** String representing the name of the entity (Maximum 11 characters). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FString Marking;
	/** The entity's linear velocity in meters per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FVector EntityLinearVelocity;
	/** The dead reckoning algorithm to use for the entity accompanied by the required additional parameters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FDeadReckoningParameters DeadReckoningParameters;
	/** The type of the entity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FEntityType EntityType;
	/** A series of enumerations used to describe the appearance of the entity according to SISO-REF-010-2015 UIDs 31-43. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FEntityAppearance EntityAppearance;
	/** A series of enumerations used to describe the capabilities of the entity according to SISO-REF-010-2015 UID 55. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		EEntityCapabilities Capabilities;
	/** The type of the entity as it should appear to entities of other forces. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		FEntityType AlternativeEntityType;
	/** A set of parameter values for each variable parameter record that is included. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityState")
		TArray<FArticulationParameters> ArticulationParameters;

	FEntityStatePDU() : FEntityInformationFamilyPDU()
	{
		PduType = EPDUType::EntityState;

		ForceID = EForceID::Other;
		Marking = FString("");
		EcefLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLinearVelocity = FVector(0, 0, 0);
		Capabilities = EEntityCapabilities::LandPlatformEntityCapabilities;
		ArticulationParameters = TArray<FArticulationParameters>();
	}

	virtual ~FEntityStatePDU() {}

	void SetupFromOpenDIS(const DIS::EntityStatePdu& EntityStatePDUIn)
	{
		FEntityInformationFamilyPDU::SetupFromOpenDIS(EntityStatePDUIn);

		// Entity State PDU specifics
		//entity id
		EntityID = EntityStatePDUIn.getEntityID();

		//location
		EcefLocation[0] = EntityStatePDUIn.getEntityLocation().getX();
		EcefLocation[1] = EntityStatePDUIn.getEntityLocation().getY();
		EcefLocation[2] = EntityStatePDUIn.getEntityLocation().getZ();

		//rotation
		const DIS::Orientation& rotation = EntityStatePDUIn.getEntityOrientation();
		EntityOrientation.Yaw = rotation.getPsi();
		EntityOrientation.Roll = rotation.getPhi();
		EntityOrientation.Pitch = rotation.getTheta();

		//velocity (originally in float so this is fine)
		EntityLinearVelocity[0] = EntityStatePDUIn.getEntityLinearVelocity().getX();
		EntityLinearVelocity[1] = EntityStatePDUIn.getEntityLinearVelocity().getY();
		EntityLinearVelocity[2] = EntityStatePDUIn.getEntityLinearVelocity().getZ();

		//Dead reckoning
		DeadReckoningParameters = EntityStatePDUIn.getDeadReckoningParameters();

		//single vars
		ForceID = static_cast<EForceID>(EntityStatePDUIn.getForceId());
		Marking = FString(EntityStatePDUIn.getMarking().getCharacters());
		Marking.LeftInline(11);
		EntityAppearance = EntityStatePDUIn.getEntityAppearance();
		Capabilities = static_cast<EEntityCapabilities>(EntityStatePDUIn.getCapabilities());

		//Entity type
		EntityType = EntityStatePDUIn.getEntityType();

		//Alternative Entity type
		AlternativeEntityType = EntityStatePDUIn.getAlternativeEntityType();

		//Articulation Parameters
		ArticulationParameters.Empty();
		for (int i = 0; i < EntityStatePDUIn.getNumberOfArticulationParameters(); i++)
		{
			ArticulationParameters.Add(EntityStatePDUIn.getArticulationParameters()[i]);
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
		OutLocation.setX(EcefLocation[0]);
		OutLocation.setY(EcefLocation[1]);
		OutLocation.setZ(EcefLocation[2]);
		EntityStatePDUOut.setEntityLocation(OutLocation);

		DIS::Orientation OutOrientation;
		OutOrientation.setPsi(EntityOrientation.Yaw);
		OutOrientation.setTheta(EntityOrientation.Pitch);
		OutOrientation.setPhi(EntityOrientation.Roll);
		EntityStatePDUOut.setEntityOrientation(OutOrientation);

		EntityStatePDUOut.setEntityAppearance(EntityAppearance.UpdateValue());
		EntityStatePDUOut.setDeadReckoningParameters(DeadReckoningParameters.ToOpenDIS());

		DIS::Marking OutMarking;
		OutMarking.setCharacterSet(1);
		OutMarking.setCharacters(TCHAR_TO_ANSI(*Marking.Left(11)));
		EntityStatePDUOut.setMarking(OutMarking);

		EntityStatePDUOut.setCapabilities(static_cast<int>(Capabilities));

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

	FEntityStateUpdatePDU ToEntityStateUpdatePDU()
	{
		FEntityStateUpdatePDU newESUPDU = FEntityStateUpdatePDU();

		//pdu common parameters
		newESUPDU.ProtocolVersion = ProtocolVersion;
		newESUPDU.ExerciseID = ExerciseID;
		newESUPDU.ProtocolFamily = ProtocolFamily;
		newESUPDU.Timestamp = Timestamp;
		newESUPDU.Length = Length;
		newESUPDU.Padding = Padding;

		//Entity State Update common parameters
		newESUPDU.EntityID = EntityID;
		newESUPDU.EcefLocation = EcefLocation;
		newESUPDU.EntityOrientation = EntityOrientation;
		newESUPDU.EntityLinearVelocity = EntityLinearVelocity;
		newESUPDU.EntityAppearance = EntityAppearance;
		newESUPDU.ArticulationParameters = ArticulationParameters;

		return newESUPDU;
	}

	FEntityStatePDU& operator = (const FEntityStateUpdatePDU& EntityStateUpdatePDUIn)
	{
		//pdu common parameters
		ProtocolVersion = EntityStateUpdatePDUIn.ProtocolVersion;
		ExerciseID = EntityStateUpdatePDUIn.ExerciseID;
		ProtocolFamily = EntityStateUpdatePDUIn.ProtocolFamily;
		Timestamp = EntityStateUpdatePDUIn.Timestamp;
		Length = EntityStateUpdatePDUIn.Length;
		Padding = EntityStateUpdatePDUIn.Padding;

		//Entity State Update common parameters
		EntityID = EntityStateUpdatePDUIn.EntityID;
		EcefLocation = EntityStateUpdatePDUIn.EcefLocation;
		EntityOrientation = EntityStateUpdatePDUIn.EntityOrientation;
		EntityLinearVelocity = EntityStateUpdatePDUIn.EntityLinearVelocity;
		EntityAppearance = EntityStateUpdatePDUIn.EntityAppearance;
		ArticulationParameters = EntityStateUpdatePDUIn.ArticulationParameters;

		return *this;
	}
};