// Copyright Epic Games, Inc. All Rights Reserved.

#include "DISEditor.h"
#include "DISEditorStyle.h"
#include "DISEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "AssetTypeActions_Base.h"
#include "DISEnumerationMappingsFactory.h"

#include "ToolMenus.h"
#include "LevelEditor.h"

static const FName UEOpenDISEditorTabName("DISEditor");

#define LOCTEXT_NAMESPACE "FDISEditor"

void FDISEditorModule::StartupModule()
{
	FDISEditorStyle::Initialize();
	FDISEditorStyle::ReloadTextures();

	FDISEditorCommands::Register();

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TSharedRef<IAssetTypeActions> ACT_UDISEnumerationMappingsDatabase = MakeShareable(new UDISEnumerationMappingsDatabase);
	AssetTools.RegisterAssetTypeActions(ACT_UDISEnumerationMappingsDatabase);
}

void FDISEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FDISEditorStyle::Shutdown();

	FDISEditorCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDISEditorModule, DISEditor)