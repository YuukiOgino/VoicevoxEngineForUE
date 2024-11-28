// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUECoreEditor.h
 * @brief  VoicevoxUECoreのEditorモジュールヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class FVoicevoxUECoreEditorModule
 * @brief VoicevoxUECore Editorモジュール
 */
class FVoicevoxUECoreEditorModule : public IModuleInterface
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
