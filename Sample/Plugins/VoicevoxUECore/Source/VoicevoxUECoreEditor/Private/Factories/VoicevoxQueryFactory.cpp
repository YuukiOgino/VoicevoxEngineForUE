// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @brief  Voicevox関連のデータアセットを生成するためのCPPファイル
 * @author Yuuki Ogino
 */

#include "Factories/VoicevoxQueryFactory.h"
#include "JsonObjectConverter.h"

/**
 * @brief コンストラクタ
 */
UVoicevoxQueryFactory::UVoicevoxQueryFactory(): Super()
{
	bCreateNew = true;
	SupportedClass = UVoicevoxQuery::StaticClass();
	bEditorImport = true;
	bText = true;
	Formats.Add(TEXT("json; AudioQueary"));
}

/**
 * @brief FactoryCreateNew override
 */	
UObject* UVoicevoxQueryFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
                                                 FName InName,  EObjectFlags Flags,
                                                 UObject* Context, FFeedbackContext* Warn)
{
	if (EditAudioQuery != nullptr)
	{
		UVoicevoxQuery* NewAudioQueryAsset = NewObject<UVoicevoxQuery>(InParent, InClass, InName, Flags, Context);
		NewAudioQueryAsset->VoicevoxAudioQuery = EditAudioQuery->VoicevoxAudioQuery;
		NewAudioQueryAsset->SpeakerType = EditAudioQuery->SpeakerType;
		NewAudioQueryAsset->Text = EditAudioQuery->Text;
		EditAudioQuery = nullptr;
		return NewAudioQueryAsset;
	}
	
	return NewObject<UVoicevoxQuery>(InParent, InClass, InName, Flags, Context);
}

/**
 * @brief FactoryCreateText override
 */	
UObject* UVoicevoxQueryFactory::FactoryCreateText(UClass* InClass, UObject* InParent, const FName InName,
                                                  const EObjectFlags Flags, UObject* Context, const TCHAR* Type,
                                                  const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	FVoicevoxAudioQuery AudioQuery{};
	FJsonObjectConverter::JsonObjectStringToUStruct(Buffer, &AudioQuery, 0, 0);
 
	UVoicevoxQuery* NewAudioQueryAsset = NewObject<UVoicevoxQuery>(InParent, InClass, InName, Flags);
	NewAudioQueryAsset->VoicevoxAudioQuery = AudioQuery;
	// インポートしたJSONにモデルデータは含まれていないので、仮でずんだもんを入れる。
	NewAudioQueryAsset->SpeakerType = 3;
	NewAudioQueryAsset->Text.Empty();
	return NewAudioQueryAsset;
}