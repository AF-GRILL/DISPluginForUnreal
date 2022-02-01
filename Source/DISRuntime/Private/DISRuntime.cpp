// Copyright Epic Games, Inc. All Rights Reserved.

#include "DISRuntime.h"

#define LOCTEXT_NAMESPACE "FDISRuntime"

void FDISRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	//Get the location of the Plugins folder and append the location of the OpenDIS6.dll within the folder.
	FString openDISPluginDir = FPaths::ProjectPluginsDir().Append("\\GRILLDISForUnreal\\Source\\ThirdParty\\Binaries\\Win64\\OpenDIS6.dll");
	DLLHandle = FPlatformProcess::GetDllHandle(*openDISPluginDir);
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