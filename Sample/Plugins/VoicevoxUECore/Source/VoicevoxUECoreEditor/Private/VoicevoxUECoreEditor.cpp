// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxUECoreEditor.h"
#include "AssetToolsModule.h"
#include "AssetTypeActions/VoicevoxQueryTypeActions.h"

#define LOCTEXT_NAMESPACE "FVoicevoxUECoreEditorModule"

void FVoicevoxUECoreEditorModule::StartupModule()
{
	if (const auto& ModuleMgr = FModuleManager::Get(); ModuleMgr.IsModuleLoaded("AssetTools"))
	{
		auto& AssetTools = ModuleMgr.LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		const auto AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("VOICEVOX")), FText::FromString(TEXT("VOICEVOX")));
		const auto Actions = MakeShareable(new FVoicevoxQueryTypeActions(AssetCategoryBit));
		AssetTools.RegisterAssetTypeActions(Actions);
	}
}

void FVoicevoxUECoreEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FVoicevoxUECoreEditorModule, VoicevoxUECoreEditor)