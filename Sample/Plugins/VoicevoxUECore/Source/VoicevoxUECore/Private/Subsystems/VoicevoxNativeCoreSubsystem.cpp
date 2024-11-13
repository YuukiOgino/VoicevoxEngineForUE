// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOX COREのネイティブライブラリのAPIを実行する基礎Subsystem CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"

DEFINE_LOG_CATEGORY(LogVoicevoxNativeCore);

/**
 * @brief VOICEVOXから受信したエラーメッセージを表示
 * @param [in] MessageFormat : エラーメッセージのフォーマット
 */
void UVoicevoxNativeCoreSubsystem::ShowVoicevoxErrorMessage(const FString& MessageFormat)
{
	  UE_LOG(LogVoicevoxNativeCore, Error, TEXT("%s"), *MessageFormat);
	  const FColor Col = FColor::Red;
	  const FVector2D Scl = FVector2D(1.0f, 1.0f);
	  GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, *MessageFormat, true, Scl);
}

