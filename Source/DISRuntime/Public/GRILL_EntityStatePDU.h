// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/EntityStatePdu.h> 
#include "GRILL_EntityStateUpdatePDU.h"
#include "GRILL_EntityStatePDU.generated.h"

USTRUCT(BlueprintType)
struct FEntityStatePDU : public FEntityInformationFamilyPdu
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

	FEntityStatePDU()
	{
		//checked
		PduType = EPDUType::EntityState;
		ForceID = EForceID::Other;
		Marking = FString("");
		EntityLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLocationDouble.Init(0, 3);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		Capabilities = 0;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_EntityStatePDU : public UGRILL_EntityInformationFamilyPDU
{
	GENERATED_BODY()
	
public:
	UGRILL_EntityStatePDU();

	void SetupFromEntityStateUpdatePDU(UGRILL_EntityStateUpdatePDU* esuPDUIn);

	void SetupFromOpenDIS(DIS::EntityStatePdu* EntityStatePDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityStatePDU EntityStatePduStruct;

	DIS::EntityStatePdu ToOpenDIS();

	virtual TArray<uint8> ToBytes() override;
};
