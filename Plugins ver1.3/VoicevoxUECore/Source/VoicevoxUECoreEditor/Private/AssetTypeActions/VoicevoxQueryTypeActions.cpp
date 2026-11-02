// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile VoicevoxQueryTypeActions.cpp
 * @brief  Voicevox関連のデータアセットをエディターから生成するためのCPPファイル
 * @author Yuuki Ogino
 */

#include "AssetTypeActions/VoicevoxQueryTypeActions.h"
#include "VoicevoxQuery.h"

/**
 * @brief コンストラクタ
 */
FVoicevoxQueryTypeActions::FVoicevoxQueryTypeActions():
	Name(FText::FromString(TEXT("Voicevox AudioQuery"))),
	AssetCategoryBit(EAssetTypeCategories::Misc)
{
}

/**
 * @brief コンストラクタ
 * @param AssetCategoryBit 
 */
FVoicevoxQueryTypeActions::FVoicevoxQueryTypeActions(EAssetTypeCategories::Type AssetCategoryBit):
	Name(FText::FromString(TEXT("Voicevox AudioQuery"))),
	AssetCategoryBit(AssetCategoryBit)
{
}

/**
 * @brief GetName 
 */	
FText FVoicevoxQueryTypeActions::GetName() const
{
	return Name;
}

/**
 * @brief GetSupportedClass 
 */	
UClass* FVoicevoxQueryTypeActions::GetSupportedClass() const
{
	return UVoicevoxQuery::StaticClass();
}

/**
 * @brief GetTypeColor 
 */	
FColor FVoicevoxQueryTypeActions::GetTypeColor() const
{
	return FColor::Purple;
}

/**
 * @brief GetCategories 
 */	
uint32 FVoicevoxQueryTypeActions::GetCategories()
{
	return AssetCategoryBit;
}

/**
 * @brief OpenAssetEditor 
 */	
void FVoicevoxQueryTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for(int i = 0; i < InObjects.Num(); ++i)
	{
		FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects[i]);
	}
};