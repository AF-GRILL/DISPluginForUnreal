// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEOpenDISEditor.h"
#include "UEOpenDISEditorStyle.h"
#include "UEOpenDISEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "UEOpenDISRuntimeSettings.h"

#include "ToolMenus.h"
#include "LevelEditor.h"

static const FName UEOpenDISEditorTabName("UEOpenDISEditor");

#define LOCTEXT_NAMESPACE "FUEOpenDISEditor"

void FUEOpenDISEditorModule::StartupModule()
{	
	FUEOpenDISEditorStyle::Initialize();
	FUEOpenDISEditorStyle::ReloadTextures();

	FUEOpenDISEditorCommands::Register();

	OpenDISProjectSettings = MakeShareable(new FUICommandList);

	//Bind the button clicked action to call the OpenDISProjectSettings_Clicked function when it is clicked
	OpenDISProjectSettings->MapAction(
		FUEOpenDISEditorCommands::Get().OpenDISProjectSettings,
		FExecuteAction::CreateRaw(this, &FUEOpenDISEditorModule::OpenDISProjectSettings_Clicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUEOpenDISEditorModule::RegisterMenus));

	// Register settings in the Project Settings menu
	SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "OpenDIS", LOCTEXT("RuntimeSettingsName", "OpenDIS"),
			LOCTEXT("RuntimeSettingsDescription", "Project settings for OpenDIS"),
			GetMutableDefault<UUEOpenDISRuntimeSettings>());
	}
}

void FUEOpenDISEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FUEOpenDISEditorStyle::Shutdown();

	FUEOpenDISEditorCommands::Unregister();

	// Unregister settings
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "OpenDIS");
	}
}

void FUEOpenDISEditorModule::OpenDISProjectSettings_Clicked() 
{
	//Open the Project Settings at the OpenDIS plugins section
	if (SettingsModule != nullptr)
	{
		SettingsModule->ShowViewer("Project", "Plugins", "OpenDIS");
	}
}

void FUEOpenDISEditorModule::RegisterMenus()
{

	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FLevelEditorModule* pLevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>(FName(TEXT("LevelEditor")));

	//Make section for the OpenDIS button to go in -- The below code will place the button after the 'Settings' section in the editor toolbar
	TSharedPtr<FExtender> pToolbarExtender = MakeShared<FExtender>();
	pToolbarExtender->AddToolBarExtension(
		"Settings",
		EExtensionHook::After,
		OpenDISProjectSettings,
		FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& builder) {
		builder.BeginSection("OpenDIS");
		builder.AddToolBarButton(FUEOpenDISEditorCommands::Get().OpenDISProjectSettings);
		builder.EndSection();
	}));

	pLevelEditorModule->GetToolBarExtensibilityManager()->AddExtender(pToolbarExtender);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUEOpenDISEditorModule, UEOpenDISEditor)