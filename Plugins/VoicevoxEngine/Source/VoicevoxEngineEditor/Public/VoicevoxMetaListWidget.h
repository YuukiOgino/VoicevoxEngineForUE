// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxMetaListWidget.h
 * @brief  VoicevoxEngineのMeta情報一覧を表示するウィジェットヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "VoicevoxUEDefined.h"

/**
 * @class SVoicevoxMetaListWidget
 * @brief Meta情報一覧を表示するSlateクラス
 */
class SVoicevoxMetaListWidget : public SCompoundWidget
{
	//! メタリスト 
	const TArray<FVoicevoxMeta>* MetaArray = nullptr;
	
public:
	SLATE_BEGIN_ARGS(SVoicevoxMetaListWidget) {}
	SLATE_ARGUMENT(const TArray<FVoicevoxMeta>*, MetaArray)
SLATE_END_ARGS()

	/**
	 * @brief Construct
	 * @param InArgs 
	 */
	void Construct(const FArguments& InArgs);

};
