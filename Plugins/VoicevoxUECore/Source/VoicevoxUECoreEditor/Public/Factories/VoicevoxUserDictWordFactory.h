// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUserDictWordFactory.h
 * @brief  Voicevox ユーザー辞書データアセットを生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUserDictWord.h"
#include "Factories/Factory.h"
#include "VoicevoxUserDictWordFactory.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UVoicevoxUserDictWordFactory
 * @brief VOicevox UserDictWordデータ生成クラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UVoicevoxUserDictWordFactory : public UFactory
{
	GENERATED_BODY()
public:

	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------
	
	//! 保存用の一時データ。
	UPROPERTY()
	TObjectPtr<UVoicevoxUserDictWord> EditUserDictWord = nullptr;

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxUserDictWordFactory();

	/**
	 * @brief FactoryCreateNew override
	 */	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent,
									  FName InName, EObjectFlags Flags,
									  UObject* Context, FFeedbackContext* Warn) override;
};
