// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxQueryFactory.h
 * @brief  Voicevox関連のデータアセットを生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
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
	UVoicevoxQueryFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent,
										FName InName, EObjectFlags Flags,
										UObject* Context, FFeedbackContext* Warn) override;
};
