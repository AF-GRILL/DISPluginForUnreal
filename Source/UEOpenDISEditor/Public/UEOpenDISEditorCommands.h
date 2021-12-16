// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UEOpenDISEditorStyle.h"

class FUEOpenDISEditorCommands : public TCommands<FUEOpenDISEditorCommands>
{
public:

	FUEOpenDISEditorCommands()
		: TCommands<FUEOpenDISEditorCommands>(TEXT("UEOpenDISEditor"), NSLOCTEXT("Contexts", "UEOpenDISEditor", "UEOpenDISEditor Plugin"), NAME_None, FUEOpenDISEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenDISProjectSettings;
};
