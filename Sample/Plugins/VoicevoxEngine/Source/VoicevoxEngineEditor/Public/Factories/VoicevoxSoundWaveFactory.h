// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "VoicevoxSoundWaveFactory.generated.h"

/**
* @class UVoicevoxSoundWaveFactory
 * @brief VOicevoxから生成した音声データを元にSoundWaveを生成する生成クラス
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UVoicevoxSoundWaveFactory : public UFactory
{
	GENERATED_BODY()
public:

	TArray<uint8> OutputWAV;
	
	UVoicevoxSoundWaveFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent,
									  FName InName, EObjectFlags Flags,
									  UObject* Context, FFeedbackContext* Warn) override;

};
