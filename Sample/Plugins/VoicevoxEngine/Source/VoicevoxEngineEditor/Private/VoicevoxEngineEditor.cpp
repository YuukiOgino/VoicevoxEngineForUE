// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxEngineEditor.h"
#include "AssetToolsModule.h"
#include "AssetTypeActions/VoicevoxQueryTypeActions.h"

#define LOCTEXT_NAMESPACE "FVoicevoxEngineEditorModule"

void FVoicevoxEngineEditorModule::StartupModule()
{
	if (const auto& ModuleMgr = FModuleManager::Get(); ModuleMgr.IsModuleLoaded("AssetTools"))
	{
		auto& AssetTools = ModuleMgr.LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		const auto AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Voicevox")), LOCTEXT("Voicevox", "AudioQueary"));
		const auto Actions = MakeShareable(new FVoicevoxQueryTypeActions(AssetCategoryBit));
		AssetTools.RegisterAssetTypeActions(Actions);
	}
}

void FVoicevoxEngineEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FVoicevoxEngineEditorModule, VoicevoxEngineEditor)