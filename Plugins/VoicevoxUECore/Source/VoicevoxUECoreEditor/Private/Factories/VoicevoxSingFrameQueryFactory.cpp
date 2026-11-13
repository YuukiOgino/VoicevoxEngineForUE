// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @brief  VVoicevoxのソング機能で使用するフレームごとの音声合成用のクエリデータアセットを生成するためのCPPファイル
 * @author Yuuki Ogino
 */

#include "Factories/VoicevoxSingFrameQueryFactory.h"
#include "Utilities/VoicevoxJsonObjectConverter.h"

/**
 * @brief コンストラクタ
 */
UVoicevoxSingFrameQueryFactory::UVoicevoxSingFrameQueryFactory(): Super()
{
	bCreateNew = true;
	SupportedClass = UVoicevoxSingFrameQuery::StaticClass();
	bEditorImport = true;
	bText = true;
	Formats.Add(TEXT("json; FrameAudioQuery"));
}

/**
 * @brief FactoryCreateNew override
 */	
UObject* UVoicevoxSingFrameQueryFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
												 FName InName,  EObjectFlags Flags,
												 UObject* Context, FFeedbackContext* Warn)
{
	if (EditScore != nullptr)
	{
		UVoicevoxSingFrameQuery* NewScoreAsset = NewObject<UVoicevoxSingFrameQuery>(InParent, InClass, InName, Flags, Context);
		NewScoreAsset->FrameAudioQuery = EditScore->FrameAudioQuery;
		NewScoreAsset->StyleId = EditScore->StyleId;
		EditScore = nullptr;
		return NewScoreAsset;
	}
	
	return NewObject<UVoicevoxSingFrameQuery>(InParent, InClass, InName, Flags, Context);
}

/**
 * @brief FactoryCreateText override
 */	
UObject* UVoicevoxSingFrameQueryFactory::FactoryCreateText(UClass* InClass, UObject* InParent, const FName InName,
												  const EObjectFlags Flags, UObject* Context, const TCHAR* Type,
												  const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	const FVoicevoxFrameAudioQuery FrameAudioQuery = FVoicevoxJsonObjectConverter::JsonObjectStringToxFrameAudioQuery(Buffer);
 
	UVoicevoxSingFrameQuery* NewScoreAsset = NewObject<UVoicevoxSingFrameQuery>(InParent, InClass, InName, Flags);
	NewScoreAsset->FrameAudioQuery = FrameAudioQuery;
	// インポートしたJSONにモデルデータは含まれていないので、仮で波音リツ（Sing）を入れる。
	NewScoreAsset->StyleId = 6000;
	return NewScoreAsset;
}