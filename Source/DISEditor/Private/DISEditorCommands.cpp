// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#include "DISEditorCommands.h"

#define LOCTEXT_NAMESPACE "FDISEditorModule"

void FDISEditorCommands::RegisterCommands()
{
	UI_COMMAND(DISProjectSettings, "GRILL DIS", "Open the GRILL DIS for Unreal plugin Project Settings", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
