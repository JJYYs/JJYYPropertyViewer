// Copyright Epic Games, Inc. All Rights Reserved.

#include "PropertyViewerCommands.h"

#define LOCTEXT_NAMESPACE "FPropertyViewerModule"

void FPropertyViewerCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "PropertyViewer", "Execute PropertyViewer action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
