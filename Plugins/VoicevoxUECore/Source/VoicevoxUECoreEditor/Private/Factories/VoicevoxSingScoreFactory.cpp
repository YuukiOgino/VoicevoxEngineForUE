// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @brief  VVoicevoxのソング機能で使用する楽譜のデータアセットを生成するためのCPPファイル
 * @author Yuuki Ogino
 */
#include "Factories/VoicevoxSingScoreFactory.h"

#include "JsonObjectConverter.h"

/**
 * @brief コンストラクタ
 */
UVoicevoxSingScoreFactory::UVoicevoxSingScoreFactory(): Super()
{
	bCreateNew = true;
	SupportedClass = UVoicevoxSingScore::StaticClass();
	bEditorImport = true;
	bText = true;
	Formats.Add(TEXT("json; Score"));
}

/**
 * @brief FactoryCreateNew override
 */	
UObject* UVoicevoxSingScoreFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
												 FName InName,  EObjectFlags Flags,
												 UObject* Context, FFeedbackContext* Warn)
{
	if (EditScore != nullptr)
	{
		UVoicevoxSingScore* NewScoreAsset = NewObject<UVoicevoxSingScore>(InParent, InClass, InName, Flags, Context);
		NewScoreAsset->Score = EditScore->Score;
		EditScore = nullptr;
		return NewScoreAsset;
	}
	
	UVoicevoxSingScore* Object = NewObject<UVoicevoxSingScore>(InParent, InClass, InName, Flags, Context);
	// 休符ノードをデフォルトとして追加
	Object->Score.Notes.Add({.ID = TEXT(""), .Key = -1, .Frame_Length = 45, .Lyric = TEXT("")});
	return Object;
}

/**
 * @brief FactoryCreateText override
 */	
UObject* UVoicevoxSingScoreFactory::FactoryCreateText(UClass* InClass, UObject* InParent, const FName InName,
												  const EObjectFlags Flags, UObject* Context, const TCHAR* Type,
												  const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	FVoicevoxScore Score{};
	FJsonObjectConverter::JsonObjectStringToUStruct(Buffer, &Score, 0, 0);
 
	UVoicevoxSingScore* NewScoreAsset = NewObject<UVoicevoxSingScore>(InParent, InClass, InName, Flags);
	NewScoreAsset->Score = Score;
	return NewScoreAsset;
}