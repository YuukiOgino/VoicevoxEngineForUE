// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxEngineEditor.h
 * @brief  VoicevoxEngineのEditorモジュールヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Modules/ModuleManager.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class FVoicevoxEngineEditorModule
 * @brief VoicevoxEngine Editoモジュール
 */
class FVoicevoxEngineEditorModule : public IModuleInterface
{
	/**
	 * @brief メインメニューに登録
	 */
	void RegisterMenus() const;

	/**
	 * @brief 既存メニューに追加
	 * @param MenuBuilder 
	 */
	void AddMenuEntry(FMenuBuilder& MenuBuilder);

	/**
	 * @brief VOICEVOX Editorを開く
	 */
	void OpenVoicevoxEditor() const;


	/**
	 * @brief VOICEVOX Speaker Listを開く
	 */
	void OpenVoicevoxMetaList() const;

public:
    /**
     * @brief StartupModule
     */
    virtual void StartupModule() override;
     
    /**
     * @brief ShutdownModule
     */
    virtual void ShutdownModule() override;
};
