// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID EntityID;
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
		FVector EntityLinearVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 EntityAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Padding1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FArticulationParameters> ArticulationParameters;

	FEntityStateUpdatePDU() : FEntityInformationFamilyPDU()
	{
		PduType = EPDUType::EntityStateUpdate;

		EntityLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLocationDouble.Init(0, 3);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		Padding1 = 0;
		ArticulationParameters = TArray<FArticulationParameters>();
	}

	virtual ~FEntityStateUpdatePDU() {}

	void SetupFromOpenDIS(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn)
	{
		FEntityInformationFamilyPDU::SetupFromOpenDIS(EntityStateUpdatePDUIn);

		//Entity State Update specifics
		//entity id
		EntityID = EntityStateUpdatePDUIn->getEntityID();

		//pure since unsupported in BP
		DIS::Vector3Double& position = EntityStateUpdatePDUIn->getEntityLocation();
		EntityLocationDouble[0] = position.getX();
		EntityLocationDouble[1] = position.getY();
		EntityLocationDouble[2] = position.getZ();

		//location
		EntityLocation[0] = position.getX();
		EntityLocation[1] = position.getY();
		EntityLocation[2] = position.getZ();

		//rotation
		DIS::Orientation& rotation = EntityStateUpdatePDUIn->getEntityOrientation();
		EntityOrientation.Yaw = rotation.getPsi();
		EntityOrientation.Roll = rotation.getPhi();
		EntityOrientation.Pitch = rotation.getTheta();

		//velocity (originally in float so this is fine)
		EntityLinearVelocity[0] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getX();
		EntityLinearVelocity[1] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getY();
		EntityLinearVelocity[2] = EntityStateUpdatePDUIn->getEntityLinearVelocity().getZ();

		//Single Vars
		Padding1 = EntityStateUpdatePDUIn->getPadding1();
		EntityAppearance = EntityStateUpdatePDUIn->getEntityAppearance();

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
		EntityStateUpdatePDUOut.setEntityLocation(OutLocation);

		DIS::Orientation OutOrientation;
		OutOrientation.setPsi(EntityOrientation.Yaw);
		OutOrientation.setTheta(EntityOrientation.Pitch);
		OutOrientation.setPhi(EntityOrientation.Roll);
		EntityStateUpdatePDUOut.setEntityOrientation(OutOrientation);

		EntityStateUpdatePDUOut.setEntityAppearance(EntityAppearance);

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