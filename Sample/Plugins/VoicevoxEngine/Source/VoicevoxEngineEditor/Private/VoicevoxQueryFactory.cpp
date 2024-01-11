// Copyright Yuuki Ogino. All Rights Reserved


#include "VoicevoxQueryFactory.h"

#include "JsonObjectConverter.h"
#include "VoicevoxQuery.h"

UVoicevoxQueryFactory::UVoicevoxQueryFactory(): Super(), SpeakerType(3)
{
	bCreateNew = true;
	SupportedClass = UVoicevoxQuery::StaticClass();
	bEditorImport = true;
	bText = true;
	Formats.Add(TEXT("json; AudioQueary"));
}

UObject* UVoicevoxQueryFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
                                                 FName InName,  EObjectFlags Flags,
                                                 UObject* Context, FFeedbackContext* Warn)
{
	if (AudioQueryPtr != nullptr)
	{
		UVoicevoxQuery* NewAudioQueryAsset = NewObject<UVoicevoxQuery>(InParent, InClass, InName, Flags, Context);
		NewAudioQueryAsset->VoicevoxAudioQuery = *AudioQueryPtr;
		NewAudioQueryAsset->SpeakerType = SpeakerType;
		AudioQueryPtr = nullptr;
		return NewAudioQueryAsset;
	}
	else
	{
		return NewObject<UVoicevoxQuery>(InParent, InClass, InName, Flags, Context);
	}
	
}

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
	return NewAudioQueryAsset;
}