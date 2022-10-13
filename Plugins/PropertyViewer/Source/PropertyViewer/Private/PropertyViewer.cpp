// Copyright Epic Games, Inc. All Rights Reserved.

#include "PropertyViewer.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "PropertyViewerStyle.h"
#include "PropertyViewerCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "WidgetBlueprint.h"

static const FName PropertyViewerTabName("PropertyViewer");

#define LOCTEXT_NAMESPACE "FPropertyViewerModule"

void FPropertyViewerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPropertyViewerStyle::Initialize();
	FPropertyViewerStyle::ReloadTextures();

	FPropertyViewerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPropertyViewerCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FPropertyViewerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPropertyViewerModule::RegisterMenus));
}

void FPropertyViewerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FPropertyViewerStyle::Shutdown();

	FPropertyViewerCommands::Unregister();
}

void FPropertyViewerModule::PluginButtonClicked()
{
	
	FString WidgetPath = TEXT("EditorUtilityWidgetBlueprint'/PropertyViewer/PropertyViewer.PropertyViewer'");
	UWidgetBlueprint* WidgetBlueprint = (UWidgetBlueprint*)StaticLoadObject(UBlueprint::StaticClass(), nullptr, *WidgetPath, nullptr, LOAD_None, nullptr);
		
	if (WidgetBlueprint)
	{
		if (WidgetBlueprint->GeneratedClass->IsChildOf(UEditorUtilityWidget::StaticClass()))
		{
			UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(WidgetBlueprint);
			if (EditorWidget)
			{
				UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
				EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidget);
			}
		}
	}
}

void FPropertyViewerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
		UToolMenu* ToolsMenu = Menu->AddSubMenu(FToolMenuOwner(Menu->GetFName()),FName("JJYYTools"),FName("JJYYTools"),FText::FromString("JJYYTools"));
		FToolMenuSection& Section = ToolsMenu->FindOrAddSection("JJYYTools");
		Section.AddMenuEntryWithCommandList(FPropertyViewerCommands::Get().PluginAction, PluginCommands);
		
	}
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPropertyViewerModule, PropertyViewer)