// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxQueryFactory.h
 * @brief  Voicevox関連のデータアセットを生成するためのヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "VoicevoxQuery.h"
#include "VoicevoxQueryFactory.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UVoicevoxQueryFactory
 * @brief VOicevox AudioQueryデータ生成クラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UVoicevoxQueryFactory : public UFactory
{
	GENERATED_BODY()
public:

	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------
	
	//! 保存用の一時データ。
	UPROPERTY()
	TObjectPtr<UVoicevoxQuery> EditAudioQuery = nullptr;

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxQueryFactory();

	/**
	 * @brief FactoryCreateNew override
	 */	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent,
	                                  FName InName, EObjectFlags Flags,
	                                  UObject* Context, FFeedbackContext* Warn) override;

	/**
	 * @brief FactoryCreateText override
	 */	
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent,
	                                   FName InName, EObjectFlags Flags,
	                                   UObject* Context, const TCHAR* Type, const TCHAR*& Buffer,
	                                   const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
};