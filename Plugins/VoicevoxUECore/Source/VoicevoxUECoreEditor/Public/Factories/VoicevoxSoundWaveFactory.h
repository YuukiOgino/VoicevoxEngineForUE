// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxSoundWaveFactory.h
 * @brief  VOICEVOXで変換したサウンドデータを元にSoundWaveを生成するヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "VoicevoxSoundWaveFactory.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
* @class UVoicevoxSoundWaveFactory
 * @brief Voicevoxから生成した音声データを元にSoundWaveを生成する生成クラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UVoicevoxSoundWaveFactory : public UFactory
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