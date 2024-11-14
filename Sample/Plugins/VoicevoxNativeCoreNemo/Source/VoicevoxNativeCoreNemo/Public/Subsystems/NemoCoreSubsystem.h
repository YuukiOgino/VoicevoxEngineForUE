// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile NemoCoreSubsystem.h
 * @brief VOICEVOX NEMO COREのAPIをまとめたSubsystemヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"
#include "NemoCoreSubsystem.generated.h"

//----------------------------------------------------------------
// class
//----------------------------------------------------------------

/**
 * @class UNemoCoreSubsystem
 * @brief VOICEVOX NEMO COREのAPIをまとめたSubsystem
 */
UCLASS(MinimalAPI)
class UNemoCoreSubsystem final : public UVoicevoxNativeCoreSubsystem
{
	GENERATED_BODY()
protected:
	
	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------

	/**
	 * @brief OpenJtakeのディレクトリ名を取得
	 * @return OpneJtakeのディレクトリ名
	 */
	virtual FString GetOpenJtakeDirectoryName() override;
	
public:

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	//--------------------------------
	// コンストラクタ
	//--------------------------------

	/**
	 * @brief コンストラクタ
	 */
	UNemoCoreSubsystem() = default;

	//--------------------------------
	// override
	//--------------------------------
	
	/**
	 * @brief Initialize
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Deinitialize
	 */
	virtual void Deinitialize() override;
	
	//--------------------------------
	// VOICEVOX CORE Property関連
	//--------------------------------
	
	/**
	 * @fn
	 *  VOICEVOX CORE名取得
	 * @brief VOICEVOX COREの名前取得
	 * @return VOICEVOX COREの名前取得
	 */
	virtual FString GetVoicevoxCoreName() override;
};
