// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUEEditorDefined.h
 * @brief  VOICEVOXをUE5 Editor内で共通に使用する構造体、列挙体の定義をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "VoicevoxUEEditorDefined.generated.h"

//------------------------------------------------------------------------
// enum
//------------------------------------------------------------------------

/**
 * @enum EAudioQueryParameter
 * @brief AudioQueryのパラメータを示す列挙体
 */
UENUM(BlueprintType)
enum class EAudioQueryParameter : uint8
{
	  SpeedScale			UMETA(DisplayName = "話速",		ToolTip = "話速"),
	  PitchScale			UMETA(DisplayName = "音高",		ToolTip = "音高"),
	  IntonationScale		UMETA(DisplayName = "抑揚",		ToolTip = "抑揚"),
	  VolumeScale			UMETA(DisplayName = "音量",		ToolTip = "音量"),
	  PrePhonemeLength		UMETA(DisplayName = "開始無音",	ToolTip = "開始無音"),
	  PostPhonemeLength		UMETA(DisplayName = "終了無音",	ToolTip = "終了無音"),
};