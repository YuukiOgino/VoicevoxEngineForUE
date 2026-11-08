// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile VoicevoxSingFrameQueryTypeActions.cpp
 * @brief  Voicevoxのソング機能で使用するフレームごとの音声合成用のクエリデータアセットをエディターから生成するためのCPPファイル
 * @author Yuuki Ogino
 */

#include "AssetTypeActions/VoicevoxSingFrameQueryTypeActions.h"
#include "VoicevoxSingFrameQuery.h"

/**
 * @brief コンストラクタ
 */
FVoicevoxSingFrameQueryTypeActions::FVoicevoxSingFrameQueryTypeActions():
	Name(FText::FromString(TEXT("Voicevox FrameAudioQuery"))),
	AssetCategoryBit(EAssetTypeCategories::Misc)
{
}

/**
 * @brief コンストラクタ
 * @param AssetCategoryBit 
 */
FVoicevoxSingFrameQueryTypeActions::FVoicevoxSingFrameQueryTypeActions(EAssetTypeCategories::Type AssetCategoryBit):
	Name(FText::FromString(TEXT("Voicevox FrameAudioQuery"))),
	AssetCategoryBit(AssetCategoryBit)
{
}

/**
 * @brief GetName 
 */	
FText FVoicevoxSingFrameQueryTypeActions::GetName() const
{
	return Name;
}

/**
 * @brief GetSupportedClass 
 */	
UClass* FVoicevoxSingFrameQueryTypeActions::GetSupportedClass() const
{
	return UVoicevoxSingFrameQuery::StaticClass();
}

/**
 * @brief GetTypeColor 
 */	
FColor FVoicevoxSingFrameQueryTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

/**
 * @brief GetCategories 
 */	
uint32 FVoicevoxSingFrameQueryTypeActions::GetCategories()
{
	return AssetCategoryBit;
}

/**
 * @brief OpenAssetEditor 
 */	
void FVoicevoxSingFrameQueryTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for(int i = 0; i < InObjects.Num(); ++i)
	{
		FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects[i]);
	}
}
