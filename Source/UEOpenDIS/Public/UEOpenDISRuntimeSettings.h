// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UEOpenDISGameState.h"
#include "UEOpenDISRuntimeSettings.generated.h"

USTRUCT()
struct FOpenDISEnumerationMappings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "OpenDISMappings")
		TAssetSubclassOf<AActor> DISEntity;

	UPROPERTY(EditAnywhere, Category = "OpenDISMappings")
		TArray<FEntityType> AssociatedDISEnumerations;
};

/**
 * 
 */
UCLASS(ClassGroup = "OpenDISRuntimeSettings", config = EditorPerProjectUserSettings)
class UEOPENDIS_API UUEOpenDISRuntimeSettings : public UObject
{
public:
	GENERATED_BODY()
		
	UUEOpenDISRuntimeSettings(const FObjectInitializer& ObjectInitializer);

	static UUEOpenDISRuntimeSettings* Get();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "OpenDISMappings",	
		Meta = (/*ConfigRestartRequired = true,*/ DisplayName = "DIS Enumeration Mappings",	Tooltip = "Mappings between DIS enumerations and associated actors."))
		TArray<FOpenDISEnumerationMappings> DISClassMappings;

private:
	static class UUEOpenDISRuntimeSettings* OpenDISSimSettingsSingleton;
};
