// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントのCPPファイル
 * @author Yuuki Ogino
 */

#include "Components/VoicevoxLipSyncAudioComponent.h"

/**
 * @brief モーフターゲット値の通知実行
 */
void UVoicevoxLipSyncAudioComponent::NotificationMorphNum(TMap<ELipSyncVowelType, float> Map)
{
	for (const auto Result : Map)
	{
		if (OnLipSyncUpdate.IsBound())
		{
			OnLipSyncUpdate.Broadcast(Result.Key, LipSyncMorphNameMap[Result.Key], Result.Value);
		}

		if (OnLipSyncUpdateNative.IsBound())
		{
			OnLipSyncUpdateNative.Broadcast(Result.Key, LipSyncMorphNameMap[Result.Key], Result.Value);
		}
	}
}