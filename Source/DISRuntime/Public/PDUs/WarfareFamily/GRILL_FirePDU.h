// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/FirePdu.h>
#include "PDUs/WarfareFamily/GRILL_WarfareFamilyPDU.h"
#include "GRILL_FirePDU.generated.h"

USTRUCT(BlueprintType)
struct FFirePDU : public FWarfareFamilyPDU
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 FireMissionIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID MunitionEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Velocity;
	UPROPERTY()
		TArray<double> LocationDouble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEventID EventID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBurstDescriptor BurstDescriptor;

	FFirePDU() : FWarfareFamilyPDU()
	{
		PduType = EPDUType::Fire;

		FireMissionIndex = 0;
		Range = 0.0;
		LocationDouble.Init(0, 3);
		Location = FVector(0, 0, 0);
		Velocity = FVector(0, 0, 0);
	}

	virtual ~FFirePDU() {}

	void SetupFromOpenDIS(DIS::FirePdu* FirePDUIn)
	{
		FWarfareFamilyPDU::SetupFromOpenDIS(FirePDUIn);

		// Fire PDU specifics
		//single vars
		FireMissionIndex = FirePDUIn->getFireMissionIndex();
		Range = FirePDUIn->getRange();

		//MunitionEntityID
		MunitionEntityID.Site = FirePDUIn->getMunitionID().getSite();
		MunitionEntityID.Application = FirePDUIn->getMunitionID().getApplication();
		MunitionEntityID.Entity = FirePDUIn->getMunitionID().getEntity();

		//velocity
		Velocity[0] = FirePDUIn->getVelocity().getX();
		Velocity[1] = FirePDUIn->getVelocity().getY();
		Velocity[2] = FirePDUIn->getVelocity().getZ();

		//location
		Location[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
		Location[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
		Location[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

		//locationDouble
		LocationDouble[0] = FirePDUIn->getLocationInWorldCoordinates().getX();
		LocationDouble[1] = FirePDUIn->getLocationInWorldCoordinates().getY();
		LocationDouble[2] = FirePDUIn->getLocationInWorldCoordinates().getZ();

		//event id
		EventID = FirePDUIn->getEventID();

		//burst descriptor
		BurstDescriptor.Warhead = FirePDUIn->getBurstDescriptor().getWarhead();
		BurstDescriptor.Fuse = FirePDUIn->getBurstDescriptor().getFuse();
		BurstDescriptor.Rate = FirePDUIn->getBurstDescriptor().getRate();
		BurstDescriptor.Quantity = FirePDUIn->getBurstDescriptor().getQuantity();
		BurstDescriptor.EntityType = FirePDUIn->getBurstDescriptor().getMunition();
	}

	void ToOpenDIS(DIS::FirePdu& FirePDUOut)
	{
		FWarfareFamilyPDU::ToOpenDIS(FirePDUOut);

		//Inherited PDU setup
		FirePDUOut.setFiringEntityID(FiringEntityID.ToOpenDIS());
		FirePDUOut.setTargetEntityID(TargetEntityID.ToOpenDIS());

		// Specific PDU setup
		FirePDUOut.setMunitionID(MunitionEntityID.ToOpenDIS());
		FirePDUOut.setFireMissionIndex(FireMissionIndex);
		FirePDUOut.setRange(Range);

		DIS::Vector3Float OutVelocity;
		OutVelocity.setX(Velocity.X);
		OutVelocity.setY(Velocity.Y);
		OutVelocity.setZ(Velocity.Z);
		FirePDUOut.setVelocity(OutVelocity);

		DIS::Vector3Double OutLocation;
		if (FMath::IsNearlyEqual(static_cast<float>(LocationDouble[0]), Location.X) &&
			FMath::IsNearlyEqual(static_cast<float>(LocationDouble[1]), Location.Y) &&
			FMath::IsNearlyEqual(static_cast<float>(LocationDouble[2]), Location.Z))
		{
			OutLocation.setX(LocationDouble[0]);
			OutLocation.setY(LocationDouble[1]);
			OutLocation.setZ(LocationDouble[2]);
		}
		else
		{
			OutLocation.setX(Location.X);
			OutLocation.setY(Location.Y);
			OutLocation.setZ(Location.Z);
		}
		FirePDUOut.setLocationInWorldCoordinates(OutLocation);

		FirePDUOut.setEventID(EventID.ToOpenDIS());
		FirePDUOut.setBurstDescriptor(BurstDescriptor.ToOpenDIS());
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::FirePdu firePDU;

		ToOpenDIS(firePDU);
		firePDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};