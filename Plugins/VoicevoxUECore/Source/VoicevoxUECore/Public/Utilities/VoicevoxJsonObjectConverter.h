// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxJsonObjectConverter.h
 * @brief VOICEVOXのJSON変換を実行するヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "VoicevoxUEDefined.h"

/**
 * @class FVoicevoxJsonObjectConverter
 * @brief VOICEVOXのJSON変換を実行するクラス
 */
class VOICEVOXUECORE_API FVoicevoxJsonObjectConverter
{
public:
	
	/**
	 * @brief AudioQueryをJSON(AudioQuery型)へ変換する
	 * @param [in] AudioQuery : 音声合成用のクエリ
	 * @param [out] OutJsonString : JSON(AudioQuery型)
	 * @return 出力処理が成功したか
	 */
	static bool AudioQueryToJsonObject(const FVoicevoxAudioQuery& AudioQuery, FString& OutJsonString);
	
	/**
	 * @brief AccentPhraseをJSON(AccentPhrase型)へ変換する
	 * @param [in] AccentPhrase : アクセント句ごとの情報
	 * @param [out] OutJsonString : JSON(AccentPhrase型)
	 * @return 出力処理が成功したか
	 */
	static bool AccentPhrasesToJsonObject(const FVoicevoxAccentPhrase& AccentPhrase, FString& OutJsonString);
	
	/**
	 * @brief AccentPhraseの配列を無名配列のJSONに変換する
	 * @param [in] AccentPhrases : AccentPhrase (アクセント句)の配列
	 * @return AccentPhraseのJSON
	 */
	static FString AccentPhraseConverterToJsonString(TArray<FVoicevoxAccentPhrase> AccentPhrases);
	
	/**
	 * @brief MoraをJSON(Mora型)へ変換する
	 * @param [in] Mora : モーラ（子音＋母音）ごとの情報
	 * @param [out] OutJsonString : JSON(Mora型)
	 * @return 出力処理が成功したか
	 */
	static bool MoraToJsonObject(const FVoicevoxMora& Mora, FString& OutJsonString);
	
	/**
	 * @brief 音符情報をJSON(Note型)に変換する
	 * @param [in] Note : 音符
	 * @param [out] OutJsonString : JSON(Note型)
	 * @return 出力処理が成功したか
	 */
	static bool NoteToJsonObject(const FVoicevoxNote& Note, FString& OutJsonString);
	
	/**
	 * @brief 楽譜情報をJSON(Score型)に変換する
	 * @param [in] Score : 楽譜情報
	 * @return JSON(Score型)
	 */
	static FString ScoreConverterToJsonString(FVoicevoxScore Score);
	
	/**
	 * @brief フレームごとの音声合成用のクエリをJSON(FrameAudioQuery型)に変換する
	 * @param [in] FrameAudioQuery : フレームごとの音声合成用のクエリ
	 * @return JSON(FrameAudioQuery型)
	 */
	static FString FrameAudioQueryConverterToJsonString(FVoicevoxFrameAudioQuery FrameAudioQuery);
	
	/**
	 * @brief JSON(FrameAudioQuery型)をフレームごとの音声合成用のクエリに変換する
	 * @param [in] JsonString : JSON(FrameAudioQuery型)
	 * @return フレームごとの音声合成用のクエリ
	 */
	static FVoicevoxFrameAudioQuery JsonObjectStringToxFrameAudioQuery(const FString& JsonString);
	
	/**
	 * @brief 音素情報をJSON(FramePhoneme型)に変換する
	 * @param [in] FramePhoneme : 音素
	 * @param [out] OutJsonString : JSON(FramePhoneme型)
	 * @return 出力処理が成功したか
	 */
	static bool FramePhonemeToJsonObject(const FVoicevoxFramePhoneme& FramePhoneme, FString& OutJsonString);
	
private:
	
	/**
	 * @brief AccentPhraseをJsonObjectへ変換する
	 * @param [in] AccentPhrase : アクセント句ごとの情報
	 * @return AccentPhraseを変換したJsonObject
	 */
	static TSharedRef<FJsonObject> AccentPhrasesToJsonObject(const FVoicevoxAccentPhrase& AccentPhrase);
	
	/**
	 * @brief MoraをJsonObjectへ変換する
	 * @param [in] Mora : モーラ（子音＋母音）ごとの情報
	 * @return Moraを変換したJsonObject
	 */
	static TSharedRef<FJsonObject> MoraToJsonObject(const FVoicevoxMora& Mora);
	
	/**
	 * @brief 音符情報をJsonObjectへ変換する
	 * @param [in] Note : 音符
	 * @return Noteを変換したJsonObject
	 */
	static TSharedRef<FJsonObject> NoteToJsonObject(const FVoicevoxNote& Note);
	
	/**
	 * @brief 音素情報をJsonObjectへ変換する
	 * @param [in] FramePhoneme : 音素
	 * @return FramePhonemeを変換したJsonObject
	 */
	static TSharedRef<FJsonObject> FramePhonemeToJsonObject(const FVoicevoxFramePhoneme& FramePhoneme);
};
