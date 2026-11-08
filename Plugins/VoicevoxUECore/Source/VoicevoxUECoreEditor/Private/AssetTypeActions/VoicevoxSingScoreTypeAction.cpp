// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile VoicevoxSingScoreTypeAction.cpp
 * @brief  Voicevoxのソング機能で使用する楽譜のデータアセットをエディターから生成するためのCPPファイル
 * @author Yuuki Ogino
 */

#include "AssetTypeActions/VoicevoxSingScoreTypeAction.h"
#include "VoicevoxSingScore.h"

/**
 * @brief コンストラクタ
 */
FVoicevoxSingScoreTypeAction::FVoicevoxSingScoreTypeAction():
	Name(FText::FromString(TEXT("Voicevox Score"))),
	AssetCategoryBit(EAssetTypeCategories::Misc)
{
}

/**
 * @brief コンストラクタ
 * @param AssetCategoryBit 
 */
FVoicevoxSingScoreTypeAction::FVoicevoxSingScoreTypeAction(EAssetTypeCategories::Type AssetCategoryBit):
	Name(FText::FromString(TEXT("Voicevox Score"))),
	AssetCategoryBit(AssetCategoryBit)
{
}

/**
 * @brief GetName 
 */	
FText FVoicevoxSingScoreTypeAction::GetName() const
{
	return Name;
}

/**
 * @brief GetSupportedClass 
 */	
UClass* FVoicevoxSingScoreTypeAction::GetSupportedClass() const
{
	return UVoicevoxSingScore::StaticClass();
}

/**
 * @brief GetTypeColor 
 */	
FColor FVoicevoxSingScoreTypeAction::GetTypeColor() const
{
	return FColor::Cyan;
}

/**
 * @brief GetCategories 
 */	
uint32 FVoicevoxSingScoreTypeAction::GetCategories()
{
	return AssetCategoryBit;
}

/**
 * @brief OpenAssetEditor 
 */	
void FVoicevoxSingScoreTypeAction::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for(int i = 0; i < InObjects.Num(); ++i)
	{
		FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects[i]);
	}
};