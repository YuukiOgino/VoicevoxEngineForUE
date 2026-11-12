// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @brief  Voicevox ユーザー辞書データアセットをエディターから生成するためのCPPファイル
 * @author Yuuki Ogino
 */
#include "AssetTypeActions/VoicevoxUserDictWordTypeActions.h"
#include "VoicevoxUserDictWord.h"

/**
 * @brief コンストラクタ
 */
FVoicevoxUserDictWordTypeActions::FVoicevoxUserDictWordTypeActions():
	Name(FText::FromString(TEXT("Voicevox UserDictionary"))),
	AssetCategoryBit(EAssetTypeCategories::Misc)
{
}

/**
 * @brief コンストラクタ
 * @param AssetCategoryBit 
 */
FVoicevoxUserDictWordTypeActions::FVoicevoxUserDictWordTypeActions(EAssetTypeCategories::Type AssetCategoryBit):
	Name(FText::FromString(TEXT("Voicevox UserDictionary"))),
	AssetCategoryBit(AssetCategoryBit)
{
}

/**
 * @brief GetName 
 */	
FText FVoicevoxUserDictWordTypeActions::GetName() const
{
	return Name;
}

/**
 * @brief GetSupportedClass 
 */	
UClass* FVoicevoxUserDictWordTypeActions::GetSupportedClass() const
{
	return UVoicevoxUserDictWord::StaticClass();
}

/**
 * @brief GetTypeColor 
 */	
FColor FVoicevoxUserDictWordTypeActions::GetTypeColor() const
{
	return FColor::Yellow;
}

/**
 * @brief GetCategories 
 */	
uint32 FVoicevoxUserDictWordTypeActions::GetCategories()
{
	return AssetCategoryBit;
}

/**
 * @brief OpenAssetEditor 
 */	
void FVoicevoxUserDictWordTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for(int i = 0; i < InObjects.Num(); ++i)
	{
		FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects[i]);
	}
};