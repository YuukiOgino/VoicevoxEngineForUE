// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @brief  Voicevox ユーザー辞書データアセットを生成するためのCPPファイル
 * @author Yuuki Ogino
 */
#include "Factories/VoicevoxUserDictWordFactory.h"

/**
 * @brief コンストラクタ
 */
UVoicevoxUserDictWordFactory::UVoicevoxUserDictWordFactory(): Super()
{
	bCreateNew = true;
	SupportedClass = UVoicevoxUserDictWord::StaticClass();
	bEditorImport = true;
	bText = true;
	Formats.Add(TEXT("json; UserDictWord"));
}

/**
 * @brief FactoryCreateNew override
 */	
UObject* UVoicevoxUserDictWordFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
												 FName InName,  EObjectFlags Flags,
												 UObject* Context, FFeedbackContext* Warn)
{
	if (EditUserDictWord != nullptr)
	{
		UVoicevoxUserDictWord* NewAudioQueryAsset = NewObject<UVoicevoxUserDictWord>(InParent, InClass, InName, Flags, Context);
		NewAudioQueryAsset->UserDictWordList = EditUserDictWord->UserDictWordList;
		EditUserDictWord = nullptr;
		return NewAudioQueryAsset;
	}
	
	return NewObject<UVoicevoxUserDictWord>(InParent, InClass, InName, Flags, Context);
}
