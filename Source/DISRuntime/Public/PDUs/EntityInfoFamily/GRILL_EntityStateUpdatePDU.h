// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/EntityStateUpdatePdu.h>
#include "PDUs/EntityInfoFamily/GRILL_EntityInformationFamilyPDU.h"
#include "GRILL_EntityStateUpdatePDU.generated.h"

USTRUCT(BlueprintType)
struct FEntityStateUpdatePDU : public FEntityInformationFamilyPDU
{
	GENERATED_BODY()

	/** The site, application, and unique identifier for this entity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateUpdate")
		FEntityID EntityID;
	/** The location of the entity in ECEF - double (64-bit) precision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateUpdate")
		FVector EcefLocation;
	/** The orientation of the entity in Psi (Yaw), Theta (Pitch), Phi (Roll) - in radians */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateUpdate")
		FRotator EntityOrientation;
	/** The entity's linear velocity in meters per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateUpdate")
		FVector EntityLinearVelocity;
	/** A series of enumerations used to describe the appearance of the entity according to SISO-REF-010-2015 UIDs 31-43. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateUpdate")
		FEntityAppearance EntityAppearance;
	/** An 8 bit field of unused padding */
	UPROPERTY()
		int32 Padding1;
	/** A set of parameter values for each variable parameter record that is included. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|EntityStateUpdate")
		TArray<FArticulationParameters> ArticulationParameters;

	FEntityStateUpdatePDU() : FEntityInformationFamilyPDU()
	{
		PduType = EPDUType::EntityStateUpdate;

		EcefLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		Padding1 = 0;
		ArticulationParameters = TArray<FArticulationParameters>();
	}

	virtual ~FEntityStateUpdatePDU() {}

	void SetupFromOpenDIS(const DIS::EntityStateUpdatePdu& EntityStateUpdatePDUIn)
	{
		FEntityInformationFamilyPDU::SetupFromOpenDIS(EntityStateUpdatePDUIn);

		//Entity State Update specifics
		//entity id
		EntityID = EntityStateUpdatePDUIn.getEntityID();

		//location
		EcefLocation[0] = EntityStateUpdatePDUIn.getEntityLocation().getX();
		EcefLocation[1] = EntityStateUpdatePDUIn.getEntityLocation().getY();
		EcefLocation[2] = EntityStateUpdatePDUIn.getEntityLocation().getZ();

		//rotation
		const DIS::Orientation& rotation = EntityStateUpdatePDUIn.getEntityOrientation();
		EntityOrientation.Yaw = rotation.getPsi();
		EntityOrientation.Roll = rotation.getPhi();
		EntityOrientation.Pitch = rotation.getTheta();

		//velocity (originally in float so this is fine)
		EntityLinearVelocity[0] = EntityStateUpdatePDUIn.getEntityLinearVelocity().getX();
		EntityLinearVelocity[1] = EntityStateUpdatePDUIn.getEntityLinearVelocity().getY();
		EntityLinearVelocity[2] = EntityStateUpdatePDUIn.getEntityLinearVelocity().getZ();

		//Single Vars
		Padding1 = EntityStateUpdatePDUIn.getPadding1();
		EntityAppearance = EntityStateUpdatePDUIn.getEntityAppearance();

		//Articulation Parameters
		for (int i = 0; i < EntityStateUpdatePDUIn.getNumberOfArticulationParameters(); i++)
		{
			DIS::ArticulationParameter tempArtParam = EntityStateUpdatePDUIn.getArticulationParameters()[i];
			FArticulationParameters newArtParam;
			newArtParam.ParameterTypeDesignator = tempArtParam.getParameterTypeDesignator();
			newArtParam.ChangeIndicator = tempArtParam.getChangeIndicator();
			newArtParam.PartAttachedTo = tempArtParam.getPartAttachedTo();
			newArtParam.ParameterType = tempArtParam.getParameterType();

			if (newArtParam.ParameterTypeDesignator == 0)
			{
				newArtParam.ParameterValue = tempArtParam.getParameterValue();
			}
			else
			{
				newArtParam.AttachedPartType = tempArtParam.getParameterValue();
			}

			ArticulationParameters.Add(newArtParam);
		}
	}

	void ToOpenDIS(DIS::EntityStateUpdatePdu& EntityStateUpdatePDUOut)
	{
		FEntityInformationFamilyPDU::ToOpenDIS(EntityStateUpdatePDUOut);

		// Specific PDU setup
		EntityStateUpdatePDUOut.setEntityID(EntityID.ToOpenDIS());
		EntityStateUpdatePDUOut.setPadding1(Padding1);

		DIS::Vector3Float OutLinearVelocity;
		OutLinearVelocity.setX(EntityLinearVelocity.X);
		OutLinearVelocity.setY(EntityLinearVelocity.Y);
		OutLinearVelocity.setZ(EntityLinearVelocity.Z);
		EntityStateUpdatePDUOut.setEntityLinearVelocity(OutLinearVelocity);

		DIS::Vector3Double OutLocation;
		OutLocation.setX(EcefLocation[0]);
		OutLocation.setY(EcefLocation[1]);
		OutLocation.setZ(EcefLocation[2]);
		EntityStateUpdatePDUOut.setEntityLocation(OutLocation);

		DIS::Orientation OutOrientation;
		OutOrientation.setPsi(EntityOrientation.Yaw);
		OutOrientation.setTheta(EntityOrientation.Pitch);
		OutOrientation.setPhi(EntityOrientation.Roll);
		EntityStateUpdatePDUOut.setEntityOrientation(OutOrientation);

		EntityStateUpdatePDUOut.setEntityAppearance(EntityAppearance.UpdateValue());

		std::vector<DIS::ArticulationParameter> OutArtParams;
		for (FArticulationParameters ArticulationParameter : ArticulationParameters)
		{
			OutArtParams.push_back(ArticulationParameter.ToOpenDIS());
		}
		EntityStateUpdatePDUOut.setArticulationParameters(OutArtParams);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::EntityStateUpdatePdu esupdu;

		ToOpenDIS(esupdu);
		esupdu.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};