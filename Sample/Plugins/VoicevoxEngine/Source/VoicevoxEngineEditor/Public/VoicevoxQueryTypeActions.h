// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxQueryTypeActions.h
 * @brief  Voicevox関連のデータアセットをエディターから生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "AssetTypeActions_Base.h"

class FVoicevoxQueryTypeActions : public FAssetTypeActions_Base
{
	FText                       Name;
	EAssetTypeCategories::Type  AssetCategoryBit;
public:

	FVoicevoxQueryTypeActions();
	FVoicevoxQueryTypeActions(EAssetTypeCategories::Type AssetCategoryBit);
	
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
								  TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
};
