// Copyright Yuuki Ogino. All Rights Reserved.

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"

DEFINE_LOG_CATEGORY(LogVoicevoxNativeCore);

UVoicevoxNativeCoreSubsystem::UVoicevoxNativeCoreSubsystem()
{
}

/**
 * @brief VOICEVOXから受信したエラーメッセージを表示
 * @param[in] MessageFormat : エラーメッセージのフォーマット
 */
void UVoicevoxNativeCoreSubsystem::ShowVoicevoxErrorMessage(const FString& MessageFormat) const
{
	  UE_LOG(LogVoicevoxNativeCore, Error, TEXT("%s"), *MessageFormat);
	  const FColor Col = FColor::Red;
	  const FVector2D Scl = FVector2D(1.0f, 1.0f);
	  GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, *MessageFormat, true, Scl);
}

