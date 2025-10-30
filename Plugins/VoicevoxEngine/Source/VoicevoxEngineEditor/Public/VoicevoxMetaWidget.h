// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxMetaWidget.h
 * @brief  VoicevoxEngineのMeta情報を表示するウィジェットヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "Widgets/SCompoundWidget.h"

/**
 * @class SVoicevoxMetaWidget
 * @brief Meta情報を表示するSlateクラス
 */
class SVoicevoxMetaWidget : public SCompoundWidget
{
	//! メタ情報
	const FVoicevoxMeta* MetaData = nullptr;
	
public:
	SLATE_BEGIN_ARGS(SVoicevoxMetaWidget) {}
	SLATE_ARGUMENT(const FVoicevoxMeta*, MetaData)
SLATE_END_ARGS()

	/**
	 * @brief Construct
	 * @param InArgs 
	 */
	void Construct(const FArguments& InArgs);
	
};
