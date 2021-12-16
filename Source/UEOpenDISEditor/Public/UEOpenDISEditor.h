// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ISettingsModule.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FUEOpenDISEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OpenDISProjectSettings_Clicked();

private:

	void RegisterMenus();

	ISettingsModule* SettingsModule;
	TSharedPtr<class FUICommandList> OpenDISProjectSettings;
};
