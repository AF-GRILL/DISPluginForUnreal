// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEOpenDISEditorCommands.h"

#define LOCTEXT_NAMESPACE "FUEOpenDISEditorModule"

void FUEOpenDISEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenDISProjectSettings, "OpenDIS", "Open the OpenDIS Project Settings", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
