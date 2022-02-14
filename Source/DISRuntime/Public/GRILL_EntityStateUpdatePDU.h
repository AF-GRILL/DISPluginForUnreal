// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <dis6/EntityStateUpdatePdu.h>
#include "GRILL_EntityInformationFamilyPDU.h"
#include "GRILL_EntityStateUpdatePDU.generated.h"

USTRUCT(BlueprintType)
struct FEntityStateUpdatePDU : public FEntityInformationFamilyPdu
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

	FEntityStateUpdatePDU()
	{
		//checked
		PduType = EPDUType::EntityStateUpdate;
		EntityLocation = FVector(0, 0, 0);
		EntityOrientation = FRotator(0, 0, 0);
		EntityLocationDouble.Init(0, 3);
		EntityLinearVelocity = FVector(0, 0, 0);
		EntityAppearance = 0;
		Padding = 0;
		Padding1 = 0;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_EntityStateUpdatePDU : public UGRILL_EntityInformationFamilyPDU
{
	GENERATED_BODY()
	
public:
	UGRILL_EntityStateUpdatePDU();

	void SetupFromOpenDIS(DIS::EntityStateUpdatePdu* EntityStateUpdatePDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityStateUpdatePDU EntityStateUpdatePduStruct;

	DIS::EntityStateUpdatePdu ToOpenDIS();

	virtual TArray<uint8> ToBytes() override;
};
