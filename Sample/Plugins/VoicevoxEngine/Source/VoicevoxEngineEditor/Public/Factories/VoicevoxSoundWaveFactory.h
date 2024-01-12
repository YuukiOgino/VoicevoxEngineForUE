// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "VoicevoxSoundWaveFactory.generated.h"

/**
* @class UVoicevoxSoundWaveFactory
 * @brief Voicevoxから生成した音声データを元にSoundWaveを生成する生成クラス
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UVoicevoxSoundWaveFactory : public UFactory
{
	GENERATED_BODY()
public:

	//! Wavデータ
	TArray<uint8> OutputWAV;

	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxSoundWaveFactory();

	/**
	 * @brief FactoryCreateNew override
	 */
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent,
									  FName InName, EObjectFlags Flags,
									  UObject* Context, FFeedbackContext* Warn) override;

};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxSoundWaveFactory, Log, All);