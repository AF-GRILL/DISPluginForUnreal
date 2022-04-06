// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ISettingsModule.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FDISEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void DISProjectSettings_Clicked();

private:

	void RegisterMenus();

	ISettingsModule* SettingsModule;
	TSharedPtr<class FUICommandList> DISProjectSettings;
};
