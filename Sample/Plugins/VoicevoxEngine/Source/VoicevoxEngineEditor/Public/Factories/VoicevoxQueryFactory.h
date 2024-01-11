// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxQueryFactory.h
 * @brief  Voicevox関連のデータアセットを生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxCoreUtil.h"
#include "Factories/Factory.h"
#include "VoicevoxQueryFactory.generated.h"

/**
 * @class UVoicevoxQueryFactory
 * @brief VOicevox AudioQueryデータ生成クラス
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UVoicevoxQueryFactory : public UFactory
{
	GENERATED_BODY()
public:

	//! 保存用の一時データ。エラーが起きるためポインタにしているが、基本参照渡し
	FVoicevoxAudioQuery* AudioQueryPtr = nullptr;

	int64 SpeakerType;

	FString Text;
	
	UVoicevoxQueryFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent,
	                                  FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;

	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent,
	                                   FName InName, EObjectFlags Flags,
	                                   UObject* Context, const TCHAR* Type, const TCHAR*& Buffer,
	                                   const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
};