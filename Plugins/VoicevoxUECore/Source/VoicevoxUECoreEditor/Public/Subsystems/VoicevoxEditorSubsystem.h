// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxEditorSubsystem.h
 * @brief  各VOICEVOX COREのAPIを呼び出すEditorSubsystemヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxApiDefined.h"
#include "VoicevoxUEDefined.h"
#include "VoicevoxEditorSubsystem.generated.h"

//----------------------------------------------------------------
// class
//----------------------------------------------------------------

/**
 * @class UVoicevoxEditorSubsystem
 * @brief 各VOICEVOX COREのAPIをEditor向けに呼び出すEngineSubsystemクラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UVoicevoxEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
	//! メタリスト
	TArray<FVoicevoxMeta> MetaList;

	//! StyleIdをキーとしたVVMファイル名Map
	TMap<int, FString> VvmFileNameMapToStyleId;
	
public:

	//--------------------------------
	// コンストラクタ
	//--------------------------------

	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxEditorSubsystem() = default;
	
	//--------------------------------
	// VOICEVOX CORE Meta関連
	//--------------------------------

	/**
	 * @brief 全てのVoicevoxVoiceModelFileからメタ情報を読み込む（Blueprint公開ノード）
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "LoadAllVvmMetaList"))
	void LoadAllVvmMetaList();
	
	/**
	 * @fn
	 * メタ情報から指定したStyleIdの名前を取得する
	 * @brief 指定したStyleIdの名前を取得する
	 * @return 指定したStyleIdのモデル名
	 */
	FString GetMetaName(VoicevoxStyleId StyleId) const;

	/**
	 * @brief 指定したStyleIdからVVMファイル名を取得する
	 * @return 指定したStyleIdが格納されたVVMファイル名
	 */
	FString GetVvmFileName(VoicevoxStyleId StyleId) const;
	
};
