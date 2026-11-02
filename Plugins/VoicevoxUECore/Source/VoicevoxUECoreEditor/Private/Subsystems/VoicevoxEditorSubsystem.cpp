// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  各VOICEVOX COREのAPIを呼び出すEditorSubsystem　cppファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/VoicevoxEditorSubsystem.h"

#include "Engine/Engine.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

//--------------------------------
// VOICEVOX CORE Meta関連
//--------------------------------

/**
 * @brief 全てのVoicevoxVoiceModelFileからメタ情報を読み込む（Blueprint公開ノード）
 */
void UVoicevoxEditorSubsystem::LoadAllVvmMetaList()
{
	MetaList.Empty();
	MetaList = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAllVoiceModelFileMetaList();
	VvmFileNameMapToStyleId = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetVvmFileNameMapToStyleId();
}

/**
 * @brief 指定したStyleIDの名前を取得する
 */
FString UVoicevoxEditorSubsystem::GetMetaName(const VoicevoxStyleId StyleId) const
{
	for  (auto [Name, Styles, Speaker_uuid, Version] : MetaList)
	{
		for (const auto Style :Styles)
		{
			if (Style.Id == StyleId)
			{
				return FString::Printf(TEXT("%s(%s)"), *Name, *Style.Name);
			}
		}
	}
	
	return TEXT("");
}

/**
 * @brief 指定したStyleIdからVVMファイル名を取得する
 */
FString UVoicevoxEditorSubsystem::GetVvmFileName(const VoicevoxStyleId StyleId) const
{
	if (VvmFileNameMapToStyleId.Contains(StyleId))
	{
		return VvmFileNameMapToStyleId[StyleId];
	}
	
	return TEXT("");
}

/**
 * @brief Subsystemが保持しているメタ情報を取得する
 */
TArray<FVoicevoxMeta> UVoicevoxEditorSubsystem::GetMetaList()
{
	return MetaList;
}