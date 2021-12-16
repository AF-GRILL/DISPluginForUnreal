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
		Meta = (DisplayName = "DIS Enumeration Mappings",	Tooltip = "Mappings between DIS enumerations and associated actors."))
		TArray<FOpenDISEnumerationMappings> DISClassMappings;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "OpenDISNetworking | Entity",
		Meta = (DisplayName = "Exercise ID", Tooltip = "The Exercise ID of the DIS sim. Valid Exercise IDs range from 0 to 255."))
		int32 ExerciseID;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "OpenDISNetworking | Entity",
		Meta = (DisplayName = "Site ID", Tooltip = "The Site ID of this application instance. Valid Site IDs range from 0 to 65535."))
		int32 SiteID;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "OpenDISNetworking | Entity",
		Meta = (DisplayName = "Application ID", Tooltip = "The Application ID of this application instance. Valid Application IDs range from 0 to 65535."))
		int32 ApplicationID;
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "OpenDISNetworking | Connection",
		Meta = (DisplayName = "IP Address", Tooltip = "The local IP address of this machine."))
		FString IPAddress = "0.0.0.0";
	UPROPERTY(GlobalConfig, EditAnywhere, Category = "OpenDISNetworking | Connection",
		Meta = (DisplayName = "Port", Tooltip = "The Port used by both Multicast and Broadcast connection types. Valid Port ranges are from 1024 to 65535."))
		int32 Port = 3000;

private:
	static class UUEOpenDISRuntimeSettings* OpenDISSimSettingsSingleton;
};
