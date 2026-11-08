// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxSingFrameQueryFactory.h
 * @brief  Voicevoxのソング機能で使用するフレームごとの音声合成用のクエリデータアセットを生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxSingFrameQuery.h"
#include "Factories/Factory.h"
#include "VoicevoxSingFrameQueryFactory.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UVoicevoxSingFrameQueryFactory
 * @brief Voicevox FrameAudioQueryデータ生成クラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UVoicevoxSingFrameQueryFactory : public UFactory
{
	GENERATED_BODY()
public:

	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------
	
	//! 保存用の一時データ。
	UPROPERTY()
	TObjectPtr<UVoicevoxSingFrameQuery> EditScore = nullptr;
	
	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxSingFrameQueryFactory();

	/**
	 * @brief FactoryCreateNew override
	 */	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent,
									  FName InName, EObjectFlags Flags,
									  UObject* Context, FFeedbackContext* Warn) override;
protected:
	/**
	 * @brief FactoryCreateText override
	 */	
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent,
									   FName InName, EObjectFlags Flags,
									   UObject* Context, const TCHAR* Type, const TCHAR*& Buffer,
									   const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
};
