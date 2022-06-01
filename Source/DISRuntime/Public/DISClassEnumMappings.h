// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DISEnumsAndStructs.h"
#include "DISInterface.h"
#include "DISClassEnumMappings.generated.h"

USTRUCT()
struct FDISClassEnumStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString FriendlyName;

	UPROPERTY(EditAnywhere, meta = (MustImplement = "DISInterface"))
		TSoftClassPtr<AActor> DISEntity;

	UPROPERTY(EditAnywhere)
		TArray<FEntityType> AssociatedDISEnumerations;
};

/**
 * 
 */
UCLASS(BlueprintType)
class DISRUNTIME_API UDISClassEnumMappings : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere,	Meta = (DisplayName = "DIS Class Enumeration Mappings", Tooltip = "Mappings between DIS enumerations and associated actors.", TitleProperty = "FriendlyName"))
		TArray<FDISClassEnumStruct> DISClassEnumArray;
};
