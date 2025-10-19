// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOX COREのAPIを実行する基礎Subsystem CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/VoicevoxApiSubsystem.h"

DEFINE_LOG_CATEGORY(LogVoicevoxApi);

/**
 * @brief VOICEVOXから受信したエラーメッセージを表示
 * @param [in] MessageFormat : エラーメッセージのフォーマット
 */
void UVoicevoxApiSubsystem::ShowVoicevoxErrorMessage(const FString& MessageFormat)
{
	UE_LOG(LogVoicevoxApi, Error, TEXT("%s"), *MessageFormat);
	const FColor Col = FColor::Red;
	const FVector2D Scl = FVector2D(1.0f, 1.0f);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, *MessageFormat, true, Scl);
}