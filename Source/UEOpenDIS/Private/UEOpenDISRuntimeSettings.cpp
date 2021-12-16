// Fill out your copyright notice in the Description page of Project Settings.


#include "UEOpenDISRuntimeSettings.h"

UUEOpenDISRuntimeSettings* UUEOpenDISRuntimeSettings::OpenDISSimSettingsSingleton = nullptr;

UUEOpenDISRuntimeSettings::UUEOpenDISRuntimeSettings(const FObjectInitializer& ObjectInitializer)
{

}

#if WITH_EDITOR

void UUEOpenDISRuntimeSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	GConfig->Flush(false);
}

#endif

UUEOpenDISRuntimeSettings* UUEOpenDISRuntimeSettings::Get()
{
	if (OpenDISSimSettingsSingleton == nullptr)
	{
		static const TCHAR* SettingsContainerName = TEXT("OpenDISRuntimeSettingsContainer");

		OpenDISSimSettingsSingleton = FindObject<UUEOpenDISRuntimeSettings>(GetTransientPackage(), SettingsContainerName);

		if (OpenDISSimSettingsSingleton == nullptr)
		{
			OpenDISSimSettingsSingleton = NewObject<UUEOpenDISRuntimeSettings>(
				GetTransientPackage(), UUEOpenDISRuntimeSettings::StaticClass(), SettingsContainerName);
			OpenDISSimSettingsSingleton->AddToRoot();
		}
	}
	return OpenDISSimSettingsSingleton;
}