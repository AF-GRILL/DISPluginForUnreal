// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DISEditorStyle.h"

class FDISEditorCommands : public TCommands<FDISEditorCommands>
{
public:

	FDISEditorCommands()
		: TCommands<FDISEditorCommands>(TEXT("DISEditor"), NSLOCTEXT("Contexts", "DISEditor", "DISEditor Plugin"), NAME_None, FDISEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > DISProjectSettings;
};
