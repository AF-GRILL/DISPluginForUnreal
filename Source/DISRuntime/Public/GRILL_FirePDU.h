// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <dis6/FirePdu.h>
#include "GRILL_WarfareFamilyPDU.h"
#include "GRILL_FirePDU.generated.h"

USTRUCT(BlueprintType)
struct FFirePDU : public FWarfareFamilyPdu
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

	FFirePDU()
	{
		PduType = EPDUType::Fire;
		FireMissionIndex = 0;
		Range = 0.0;
		LocationDouble.Init(0, 3);
		Location = FVector(0, 0, 0);
		Velocity = FVector(0, 0, 0);
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_FirePDU : public UGRILL_WarfareFamilyPDU
{
	GENERATED_BODY()
	
public:
	UGRILL_FirePDU();

	void SetupFromOpenDIS(DIS::FirePdu* FirePDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FFirePDU FirePduStruct;

	DIS::FirePdu ToOpenDIS();

	virtual TArray<uint8> ToBytes() override;	
};
