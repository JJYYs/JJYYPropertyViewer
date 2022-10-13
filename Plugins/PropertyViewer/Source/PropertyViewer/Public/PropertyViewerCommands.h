// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PropertyViewerStyle.h"

class FPropertyViewerCommands : public TCommands<FPropertyViewerCommands>
{
public:

	FPropertyViewerCommands()
		: TCommands<FPropertyViewerCommands>(TEXT("PropertyViewer"), NSLOCTEXT("Contexts", "PropertyViewer", "PropertyViewer Plugin"), NAME_None, FPropertyViewerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
