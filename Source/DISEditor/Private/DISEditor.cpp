// Copyright Epic Games, Inc. All Rights Reserved.

#include "DISEditor.h"
#include "DISEditorStyle.h"
#include "DISEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "DISRuntimeSettings.h"

#include "ToolMenus.h"
#include "LevelEditor.h"

static const FName UEOpenDISEditorTabName("DISEditor");

#define LOCTEXT_NAMESPACE "FDISEditor"

void FDISEditorModule::StartupModule()
{	
	FDISEditorStyle::Initialize();
	FDISEditorStyle::ReloadTextures();

	FDISEditorCommands::Register();

	DISProjectSettings = MakeShareable(new FUICommandList);

	//Bind the button clicked action to call the OpenDISProjectSettings_Clicked function when it is clicked
	DISProjectSettings->MapAction(
		FDISEditorCommands::Get().DISProjectSettings,
		FExecuteAction::CreateRaw(this, &FDISEditorModule::DISProjectSettings_Clicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDISEditorModule::RegisterMenus));

	// Register settings in the Project Settings menu
	SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "GRILL DIS", LOCTEXT("RuntimeSettingsName", "GRILL DIS"),
			LOCTEXT("RuntimeSettingsDescription", "Project settings for the GRILL DIS for Unreal plugin"),
			GetMutableDefault<UDISRuntimeSettings>());
	}
}

void FDISEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FDISEditorStyle::Shutdown();

	FDISEditorCommands::Unregister();

	// Unregister settings
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "GRILL DIS");
	}
}

void FDISEditorModule::DISProjectSettings_Clicked() 
{
	//Open the Project Settings at the OpenDIS plugins section
	if (SettingsModule != nullptr)
	{
		SettingsModule->ShowViewer("Project", "Plugins", "GRILL DIS");
	}
}

void FDISEditorModule::RegisterMenus()
{

	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FLevelEditorModule* pLevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>(FName(TEXT("LevelEditor")));

	//Make section for the OpenDIS button to go in -- The below code will place the button after the 'Settings' section in the editor toolbar
	TSharedPtr<FExtender> pToolbarExtender = MakeShared<FExtender>();
	pToolbarExtender->AddToolBarExtension(
		"Settings",
		EExtensionHook::After,
		DISProjectSettings,
		FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& builder) {
		builder.BeginSection("GRILL DIS");
		builder.AddToolBarButton(FDISEditorCommands::Get().DISProjectSettings);
		builder.EndSection();
	}));

	pLevelEditorModule->GetToolBarExtensibilityManager()->AddExtender(pToolbarExtender);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDISEditorModule, DISEditor)