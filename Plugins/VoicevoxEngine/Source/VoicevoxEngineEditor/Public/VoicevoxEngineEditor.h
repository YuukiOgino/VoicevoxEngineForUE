// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxEngineEditor.h
 * @brief  VoicevoxEngineのEditorモジュールヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
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
