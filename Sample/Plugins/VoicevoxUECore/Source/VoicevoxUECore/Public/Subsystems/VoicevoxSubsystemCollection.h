// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxSubsystemCollection.h
 * @brief VOICEVOX COREのネイティブAPIを参照するSubsystem管理ヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"

class UVoicevoxNativeObject;

/**
 * @class FVoicevoxSubsystemCollection
 * @brief VOICEVOX COREのネイティブAPIを参照するSubsystem管理クラス
 */
class FVoicevoxSubsystemCollection final : public FSubsystemCollectionBase
{

	//----------------------------------------------------------------
	// friend class
	//----------------------------------------------------------------
	
	friend class UVoicevoxNativeObject;

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	/**
	 * @brief 指定したクラスのSubsystemを取得
	 * @param SubsystemClass 
	 * @return 指定したVOICEVOX CORE Subsystemクラス
	 */
	USubsystem* GetSubsystem(UClass* SubsystemClass) const
	{
		return GetSubsystemInternal(SubsystemClass);
	}
	
public:
	
	/**
	 * @brief コンストラクタ
	 */
	FVoicevoxSubsystemCollection() : FSubsystemCollectionBase(UVoicevoxNativeCoreSubsystem::StaticClass()) {}
};
