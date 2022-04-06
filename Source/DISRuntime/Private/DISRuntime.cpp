// Copyright Epic Games, Inc. All Rights Reserved.

#include "DISRuntime.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FDISRuntime"

void FDISRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	//Get the location of the GRILL DIS Plugin folder
	TSharedPtr<IPlugin> GrillDIS = IPluginManager::Get().FindPlugin("GRILLDISForUnreal");

	if (GrillDIS)
	{
		//Append the location of the OpenDIS6.dll within the plugin folder
		FString openDISPluginDir = GrillDIS->GetBaseDir().Append("\\Source\\ThirdParty\\Binaries\\Win64\\OpenDIS6.dll");
		DLLHandle = FPlatformProcess::GetDllHandle(*openDISPluginDir);
	}
}

void FDISRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	//Free the loaded OpenDIS6 DLL
	FPlatformProcess::FreeDllHandle(DLLHandle);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDISRuntimeModule, DISRuntime)