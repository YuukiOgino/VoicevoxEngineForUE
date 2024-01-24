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
class FVoicevoxUECoreEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
