// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxSingScoreTypeActions.h
 * @brief  Voicevoxのソング機能で使用するフレームごとの音声合成用のクエリデータアセットをエディターから生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "AssetTypeActions_Base.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class FVoicevoxSingFrameQueryTypeActions
 * @brief Voicevoxのソング機能で使用するフレームごとの音声合成用のクエリデータアセットをエディターから生成するためのアクションクラス
 */
class FVoicevoxSingFrameQueryTypeActions : public FAssetTypeActions_Base
{
	//! 項目名
	FText                       Name;

	//! アセットカテゴリー
	EAssetTypeCategories::Type  AssetCategoryBit;
public:
	
	/**
	 * @brief コンストラクタ
	 */
	FVoicevoxSingFrameQueryTypeActions();

	/**
	 * @brief コンストラクタ
	 * @param AssetCategoryBit 
	 */
	explicit FVoicevoxSingFrameQueryTypeActions(EAssetTypeCategories::Type AssetCategoryBit);

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
