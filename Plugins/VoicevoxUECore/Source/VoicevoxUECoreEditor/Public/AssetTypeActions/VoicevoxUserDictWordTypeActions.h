// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUserDictWordTypeActions.h
 * @brief  Voicevox ユーザー辞書データアセットをエディターから生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "AssetTypeActions_Base.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class FVoicevoxUserDictWordTypeActions
 * @brief Voicevox ユーザー辞書データアセットをエディターから生成するためのアクションクラス
 */
class FVoicevoxUserDictWordTypeActions : public FAssetTypeActions_Base
{
		//! 項目名
		FText                       Name;

		//! アセットカテゴリー
		EAssetTypeCategories::Type  AssetCategoryBit;
public:
	
		/**
		 * @brief コンストラクタ
		 */
		FVoicevoxUserDictWordTypeActions();

		/**
		 * @brief コンストラクタ
		 * @param AssetCategoryBit 
		 */
		explicit FVoicevoxUserDictWordTypeActions(EAssetTypeCategories::Type AssetCategoryBit);

		/**
		 * @brief GetName　override
		 * @return Returns the name of this type
		 */
		virtual FText GetName() const override;

		/**
		 * @brief GetSupportedClass　override
		 * @return Get the supported class of this type
		 */
		virtual UClass* GetSupportedClass() const override;

		/**
		 * @brief GetTypeColor override
		 * @return Returns the color associated with this type
		 */
		virtual FColor GetTypeColor() const override;

		/**
		 * @brief GetCategories override
		 * @return Returns the categories that this asset type appears in. The return value is one or more flags from EAssetTypeCategories.
		 */	
		virtual uint32 GetCategories() override;

		/**
		 * @brief OpenAssetEditor override
		 */		
		virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
									  TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
};
