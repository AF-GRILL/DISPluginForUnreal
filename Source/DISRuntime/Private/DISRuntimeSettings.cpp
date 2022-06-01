// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.
#include "DISRuntimeSettings.h"

UDISRuntimeSettings* UDISRuntimeSettings::DISSimSettingsSingleton = nullptr;

UDISRuntimeSettings::UDISRuntimeSettings(const FObjectInitializer& ObjectInitializer)
{

}

#if WITH_EDITOR

void UDISRuntimeSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	GConfig->Flush(false);
}

#endif

UDISRuntimeSettings* UDISRuntimeSettings::Get()
{
	if (DISSimSettingsSingleton == nullptr)
	{
		static const TCHAR* SettingsContainerName = TEXT("DISRuntimeSettingsContainer");

		DISSimSettingsSingleton = FindObject<UDISRuntimeSettings>(GetTransientPackage(), SettingsContainerName);

		if (DISSimSettingsSingleton == nullptr)
		{
			DISSimSettingsSingleton = NewObject<UDISRuntimeSettings>(
				GetTransientPackage(), UDISRuntimeSettings::StaticClass(), SettingsContainerName);
			DISSimSettingsSingleton->AddToRoot();
		}
	}
	return DISSimSettingsSingleton;
}