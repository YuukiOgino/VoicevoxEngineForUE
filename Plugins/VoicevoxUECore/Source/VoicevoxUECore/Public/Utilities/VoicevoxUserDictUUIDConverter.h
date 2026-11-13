// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUserDictUUIDConverter.h
 * @brief VOICEVOXのUUID変換を実行するヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

/**
 * @class FVoicevoxUserDictUUIDConverter
 * @brief VOICEVOXのUUID変換を実行するクラス
 */
class VOICEVOXUECORE_API FVoicevoxUserDictUUIDConverter
{
public:
	/**
	 * @brief VOICEVOXから受信したUUIDをFGuid型へ変換する
	 * @param [in] UUID : ユーザー辞書登録時に取得したUUID
	 * @retun UUIDをRFC 4122規定に従った変換をかけたFGuid
	 */
	static FGuid UUIDConverterToGuid(TArray<uint8> UUID);
	
	/**
	 * @brief VOICEVOXから受信したFGuid型をUUIDへ変換する
	 * @param [in] UUID : UUIDをRFC 4122規定に従った変換をかけたFGuid
	 * @retun ユーザー辞書登録時に取得したUUID
	 */
	static TArray<uint8> GuidConverterToUUID(FGuid UUID);
};
