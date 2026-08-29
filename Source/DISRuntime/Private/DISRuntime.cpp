#include "DISRuntime.h"

#define LOCTEXT_NAMESPACE "FDISRuntime"

void FDISRuntimeModule::StartupModule()
{
	// OpenDIS is compiled into the module for all supported platforms.
}

void FDISRuntimeModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDISRuntimeModule, DISRuntime)
