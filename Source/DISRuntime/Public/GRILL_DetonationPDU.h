// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <dis6/DetonationPdu.h>
#include "GRILL_WarfareFamilyPDU.h"
#include "GRILL_DetonationPDU.generated.h"

USTRUCT(BlueprintType)
struct FDetonationPDU : public FWarfareFamilyPdu
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEntityID MunitionEntityID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEventID EventID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Velocity;
	UPROPERTY()
		TArray<double> LocationDouble;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBurstDescriptor BurstDescriptor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LocationInEntityCoords;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 DetonationResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Pad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FArticulationParameters> ArticulationParameters;

	FDetonationPDU()
	{
		PduType = EPDUType::Detonation;
		Velocity = FVector(0, 0, 0);
		LocationDouble.Init(0, 3);
		Location = FVector(0, 0, 0);
		LocationInEntityCoords = FVector(0, 0, 0);
		DetonationResult = 0U;
		Pad = 0;
	}
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UGRILL_DetonationPDU : public UGRILL_WarfareFamilyPDU
{
	GENERATED_BODY()
	
public:
	UGRILL_DetonationPDU();

	void SetupFromOpenDIS(DIS::DetonationPdu* DetonationPDUIn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDetonationPDU DetonationPduStruct;

	DIS::DetonationPdu ToOpenDIS();

	virtual TArray<uint8> ToBytes() override;
};
