// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile VoicevoxSingScoreTypeActions.cpp
 * @brief  Voicevoxのソング機能で使用する楽譜のデータアセットをエディターから生成するためのCPPファイル
 * @author Yuuki Ogino
 */

#include "AssetTypeActions/VoicevoxSingScoreTypeActions.h"
#include "VoicevoxSingScore.h"

/**
 * @brief コンストラクタ
 */
FVoicevoxSingScoreTypeActions::FVoicevoxSingScoreTypeActions():
	Name(FText::FromString(TEXT("Voicevox Score"))),
	AssetCategoryBit(EAssetTypeCategories::Misc)
{
}

/**
 * @brief コンストラクタ
 * @param AssetCategoryBit 
 */
FVoicevoxSingScoreTypeActions::FVoicevoxSingScoreTypeActions(EAssetTypeCategories::Type AssetCategoryBit):
	Name(FText::FromString(TEXT("Voicevox Score"))),
	AssetCategoryBit(AssetCategoryBit)
{
}

/**
 * @brief GetName 
 */	
FText FVoicevoxSingScoreTypeActions::GetName() const
{
	return Name;
}

/**
 * @brief GetSupportedClass 
 */	
UClass* FVoicevoxSingScoreTypeActions::GetSupportedClass() const
{
	return UVoicevoxSingScore::StaticClass();
}

/**
 * @brief GetTypeColor 
 */	
FColor FVoicevoxSingScoreTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

/**
 * @brief GetCategories 
 */	
uint32 FVoicevoxSingScoreTypeActions::GetCategories()
{
	return AssetCategoryBit;
}

/**
 * @brief OpenAssetEditor 
 */	
void FVoicevoxSingScoreTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for(int i = 0; i < InObjects.Num(); ++i)
	{
		FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects[i]);
	}
};