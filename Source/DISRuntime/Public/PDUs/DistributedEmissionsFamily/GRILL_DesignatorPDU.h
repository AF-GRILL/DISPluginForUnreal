// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/DesignatorPdu.h>
#include "PDUs/DistributedEmissionsFamily/GRILL_DistributedEmissionsFamilyPDU.h"
#include "GRILL_DesignatorPDU.generated.h"

USTRUCT(BlueprintType)
struct FDesignatorPDU : public FDistributedEmissionsFamilyPDU
{
	GENERATED_BODY()

	/** The site, application, and unique identifier for the entity that is positioning the designator. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	FEntityID DesignatingEntityID;
	/** This field shall identify the code name for the designator system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	int32 CodeName;
	/** The site, application, and unique identifier for the entity that is currently being designated. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	FEntityID DesignatedEntityID;
	/** This field shall identify the designator code being used by the designating entity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	int32 DesignatorCode;
	/** This field shall identify the designator output power in watts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	float DesignatorPower;
	/** This field shall identify the designator wavelength in units of microns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	float DesignatorWavelength;
	/** This field shall specify the location of the designator spot with respect to the designated entity's coordinate system when the spot is on an entity. If the Designated Entity is D-SPOT_NO_ENTITY, then this field shall contain the location [0, 0, 0]. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	FVector DesignatorSpotWrtDesignated;
	/** This field shall identify the location of the designator spot with respect to the world coordinate system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	FVector DesignatorSpotLocation;
	/** This field will be used to provide parameters for dead reckoning the position of the designator spot. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	EDeadReckoningAlgorithm DeadReckoningAlgorithm;
	/** Padding */
	UPROPERTY()
	int32 Padding1;
	/** Padding */
	UPROPERTY()
	uint8 Padding2;
	/** This field shall specify the designator spot's linear acceleration. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GRILL DIS|Structs|PDUs|Designator")
	FVector EntityLinearAcceleration;

	FDesignatorPDU() : FDistributedEmissionsFamilyPDU()
	{
		PduType = EPDUType::Designator;

		CodeName = 0;
		DesignatorCode = 0;
		DesignatorPower = 0;
		DesignatorWavelength = 0;
		DesignatorSpotWrtDesignated = FVector(0, 0, 0);
		DesignatorSpotLocation = FVector(0, 0, 0);
		DeadReckoningAlgorithm = EDeadReckoningAlgorithm::Static;
		Padding1 = 0;
		Padding2 = 0;
		EntityLinearAcceleration = FVector(0, 0, 0);
	}

	virtual ~FDesignatorPDU() {}

	void SetupFromOpenDIS(const DIS::DesignatorPdu& DesignatorPDUIn)
	{
		FPDU::SetupFromOpenDIS(DesignatorPDUIn);

		DesignatingEntityID = DesignatorPDUIn.getDesignatingEntityID();
		CodeName = DesignatorPDUIn.getCodeName();
		DesignatedEntityID = DesignatorPDUIn.getDesignatedEntityID();
		DesignatorCode = DesignatorPDUIn.getDesignatorCode();
		DesignatorPower = DesignatorPDUIn.getDesignatorPower();
		DesignatorWavelength = DesignatorPDUIn.getDesignatorWavelength();

		DesignatorSpotWrtDesignated.X = DesignatorPDUIn.getDesignatorSpotWrtDesignated().getX();
		DesignatorSpotWrtDesignated.Y = DesignatorPDUIn.getDesignatorSpotWrtDesignated().getY();
		DesignatorSpotWrtDesignated.Z = DesignatorPDUIn.getDesignatorSpotWrtDesignated().getZ();

		DesignatorSpotLocation.X = DesignatorPDUIn.getDesignatorSpotLocation().getX();
		DesignatorSpotLocation.Y = DesignatorPDUIn.getDesignatorSpotLocation().getY();
		DesignatorSpotLocation.Z = DesignatorPDUIn.getDesignatorSpotLocation().getZ();

		DeadReckoningAlgorithm = static_cast<EDeadReckoningAlgorithm>(DesignatorPDUIn.getDeadReckoningAlgorithm());
		Padding1 = DesignatorPDUIn.getPadding1();
		Padding2 = DesignatorPDUIn.getPadding2();

		EntityLinearAcceleration.X = DesignatorPDUIn.getEntityLinearAcceleration().getX();
		EntityLinearAcceleration.Y = DesignatorPDUIn.getEntityLinearAcceleration().getY();
		EntityLinearAcceleration.Z = DesignatorPDUIn.getEntityLinearAcceleration().getZ();
	}

	void ToOpenDIS(DIS::DesignatorPdu& DesignatorPDUOut)
	{
		FPDU::ToOpenDIS(DesignatorPDUOut);

		DesignatorPDUOut.setDesignatingEntityID(DesignatingEntityID.ToOpenDIS());
		DesignatorPDUOut.setCodeName(CodeName);
		DesignatorPDUOut.setDesignatedEntityID(DesignatedEntityID.ToOpenDIS());
		DesignatorPDUOut.setDesignatorCode(DesignatorCode);
		DesignatorPDUOut.setDesignatorPower(DesignatorPower);
		DesignatorPDUOut.setDesignatorWavelength(DesignatorWavelength);

		DIS::Vector3Float OutSpotWrtDesignated;
		OutSpotWrtDesignated.setX(DesignatorSpotWrtDesignated.X);
		OutSpotWrtDesignated.setY(DesignatorSpotWrtDesignated.Y);
		OutSpotWrtDesignated.setZ(DesignatorSpotWrtDesignated.Z);
		DesignatorPDUOut.setDesignatorSpotWrtDesignated(OutSpotWrtDesignated);

		DIS::Vector3Double OutSpotLocation;
		OutSpotLocation.setX(DesignatorSpotLocation.X);
		OutSpotLocation.setY(DesignatorSpotLocation.Y);
		OutSpotLocation.setZ(DesignatorSpotLocation.Z);
		DesignatorPDUOut.setDesignatorSpotLocation(OutSpotLocation);

		DesignatorPDUOut.setDeadReckoningAlgorithm(static_cast<unsigned char>(DeadReckoningAlgorithm));
		DesignatorPDUOut.setPadding1(Padding1);
		DesignatorPDUOut.setPadding2(Padding2);

		DIS::Vector3Float OutLinearAcceleration;
		OutLinearAcceleration.setX(EntityLinearAcceleration.X);
		OutLinearAcceleration.setY(EntityLinearAcceleration.Y);
		OutLinearAcceleration.setZ(EntityLinearAcceleration.Z);
		DesignatorPDUOut.setEntityLinearAcceleration(OutLinearAcceleration);
	}

	virtual TArray<uint8> ToBytes() override
	{
		DIS::DataStream buffer(DIS::BIG);

		//marshal
		DIS::DesignatorPdu DesignatorPDU;

		ToOpenDIS(DesignatorPDU);
		DesignatorPDU.marshal(buffer);

		return FPDU::DISDataStreamToBytes(buffer);
	}
};