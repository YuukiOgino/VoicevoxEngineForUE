// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxSingScoreFactory.h
 * @brief  Voicevoxのソング機能で使用する楽譜のデータアセットを生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxSingScore.h"
#include "Factories/Factory.h"

#include "VoicevoxSingScoreFactory.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UVoicevoxSingScoreFactory
 * @brief VOicevox Scoreデータ生成クラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UVoicevoxSingScoreFactory : public UFactory
{
	GENERATED_BODY()
public:

	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------
	
	//! 保存用の一時データ。
	UPROPERTY()
	TObjectPtr<UVoicevoxSingScore> EditScore = nullptr;
	
	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxSingScoreFactory ();

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
