// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxEngineEditor.cpp
 * @brief  VoicevoxEngineのEditorモジュールCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxEngineEditor.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "VoicevoxMetaListWidget.h"
#include "Subsystems/VoicevoxEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "FVoicevoxEngineEditorModule"

/**
 * @brief StartupModule
 */
void FVoicevoxEngineEditorModule::StartupModule()
{
	// メニュー登録をコールバックで遅延実行
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FVoicevoxEngineEditorModule::RegisterMenus)
	);

	// WindowレイアウトにVOICEVOX　Editorを開く機能を追加
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	const TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension(
		"Tools",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FVoicevoxEngineEditorModule::AddMenuEntry)
	);
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

/**
 * @brief ShutdownModule
 */
void FVoicevoxEngineEditorModule::ShutdownModule()
{
	UToolMenus::UnregisterOwner(this);
}

/**
 * @brief メインメニューに登録
 */
void FVoicevoxEngineEditorModule::RegisterMenus() const
{
	// LevelEditorメインメニューを拡張
	UToolMenu* MainMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
	// VOICEVOXメニューを追加
	FToolMenuSection& ToolsSection = MainMenu->AddSection("VoicevoxSection", LOCTEXT("VoicevoxSection", "VOICEVOX"));
	
	// サブメニューを作る
	const FToolMenuEntry SubMenu = FToolMenuEntry::InitSubMenu(
		"VoicevoxSubMenu",
		LOCTEXT("VoicevoxSection", "VOICEVOX"),
		LOCTEXT("VoicevoxSMenuTooltip", "VOICEVOX Editor Tools"),
		FNewToolMenuDelegate::CreateLambda([this](UToolMenu* VoicevoxSubMenu)
		{
			FToolMenuSection& SubSection = VoicevoxSubMenu->AddSection("WidgetSection", LOCTEXT("WidgetSection", "VOICEVOX"));
			SubSection.AddMenuEntry(
				"OpenVoicevoxEditor",
				LOCTEXT("OpenVoicevoxEditor", "VOICEVOX Editor"),
				LOCTEXT("OpenVoicevoxEditor_Tooltip", "Open the VOICEVOX Editor"),
				FSlateIcon("EditorStyle", "LevelEditor.Tabs.Details"),
				FUIAction(FExecuteAction::CreateRaw(this, &FVoicevoxEngineEditorModule::OpenVoicevoxEditor))
			);

			SubSection.AddMenuEntry(
				"OpenVoicevoxSpeakerList",
				LOCTEXT("OpenVoicevoxEditor", "Speaker List"),
				LOCTEXT("OpenVoicevoxSpeakerList_Tooltip", "Open the VOICEVOX Speaker List"),
				FSlateIcon("EditorStyle", "LevelEditor.Tabs.Details"),
				FUIAction(FExecuteAction::CreateRaw(this, &FVoicevoxEngineEditorModule::OpenVoicevoxMetaList))
			);
		}),
		false, 
		FSlateIcon()
	);
	ToolsSection.AddEntry(SubMenu);
}

/**
 * @brief 既存メニューに追加
 */
void FVoicevoxEngineEditorModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddSubMenu(LOCTEXT("AddVoicevoxEditorMenu", "VOICEVOX"),
		LOCTEXT("AddVoicevoxEditorMenu_Tooltip", "VOICEVOX Editor Menu"),
		FNewMenuDelegate::CreateLambda([this](FMenuBuilder& Builder)
		{
			Builder.AddMenuEntry(
				LOCTEXT("OpenVoicevoxEditor", "VOICEVOX Editor"),
				LOCTEXT("OpenVoicevoxEditor_Tooltip", "Open the VOICEVOX Editor"),
				FSlateIcon("EditorStyle", "LevelEditor.Tabs.Details"),
				FUIAction(FExecuteAction::CreateRaw(this, &FVoicevoxEngineEditorModule::OpenVoicevoxEditor))
			);

			Builder.AddMenuEntry(
				LOCTEXT("OpenVoicevoxEditor", "Speaker List"),
				LOCTEXT("OpenVoicevoxSpeakerList_Tooltip", "Open the VOICEVOX Speaker List"),
				FSlateIcon("EditorStyle", "LevelEditor.Tabs.Details"),
				FUIAction(FExecuteAction::CreateRaw(this, &FVoicevoxEngineEditorModule::OpenVoicevoxMetaList))
			);
		}),
		false, 
		FSlateIcon()
	);
}

/**
 * @brief VOICEVOX Editorを開く
 */
void FVoicevoxEngineEditorModule::OpenVoicevoxEditor() const
{
	if (UEditorUtilitySubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
	{
		const FString WidgetPath = TEXT("/VoicevoxEngine/Editor/VoicevoxEditor.VoicevoxEditor");
		const FSoftObjectPath WidgetSoftPath(WidgetPath);

		if (UEditorUtilityWidgetBlueprint* WidgetBP = Cast<UEditorUtilityWidgetBlueprint>(WidgetSoftPath.TryLoad()))
		{
			Subsystem->SpawnAndRegisterTab(WidgetBP);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load EUW: %s"), *WidgetPath);
		}
	}
}

/**
 * @brief VOICEVOX Speaker Listを開く
 */
void FVoicevoxEngineEditorModule::OpenVoicevoxMetaList() const
{
	const auto List = GEditor->GetEditorSubsystem<UVoicevoxEditorSubsystem>()->GetMetaList();
	const TSharedRef<SWindow> Window = SNew(SWindow)
	.Title(FText::FromString(TEXT("Voicevox Speaker Info")))
	.ClientSize(FVector2D(700, 600))
	[
		SNew(SVoicevoxMetaListWidget)
		.MetaArray(&List)
	];

	FSlateApplication::Get().AddWindow(Window);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FVoicevoxEngineEditorModule, VoicevoxEngineEditor)