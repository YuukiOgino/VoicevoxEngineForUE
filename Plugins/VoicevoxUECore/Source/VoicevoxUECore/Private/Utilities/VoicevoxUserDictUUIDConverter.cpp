// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOXのUUID変換を実行するCPPファイル
 * @author Yuuki Ogino
 */
#include "Utilities/VoicevoxUserDictUUIDConverter.h"

/**
 * @brief VOICEVOXから受信したUUIDをFGuid型へ変換する
 */
FGuid FVoicevoxUserDictUUIDConverter::UUIDConverterToGuid(TArray<uint8> UUID)
{
	// JSONには反転させてRFC 4122規定に従ったハイフン区切りのようなので、処理統一のためUE側もあえて反転させる。削除、更新は元に戻す。
	Algo::Reverse(UUID);
	uint32 A, B, C, D = 0;

	FMemory::Memcpy(&D, UUID.GetData() + 0, 4);
	FMemory::Memcpy(&C, UUID.GetData() + 4, 4);
	FMemory::Memcpy(&B, UUID.GetData() + 8, 4);
	FMemory::Memcpy(&A, UUID.GetData() + 12, 4);
	
	return FGuid(A, B, C, D);
}
	
/**
 * @brief VOICEVOXから受信したFGuid型をUUIDへ変換する
 */
TArray<uint8> FVoicevoxUserDictUUIDConverter::GuidConverterToUUID(const FGuid UUID)
{
	TArray<uint8> Bytes;
	Bytes.SetNumUninitialized(16);

	FMemory::Memcpy(Bytes.GetData() + 0, &UUID.D, 4);
	FMemory::Memcpy(Bytes.GetData() + 4,  &UUID.C, 4);
	FMemory::Memcpy(Bytes.GetData() + 8,  &UUID.B, 4);
	FMemory::Memcpy(Bytes.GetData() + 12,  &UUID.A, 4);
	
	// JSONには反転させてRFC 4122規定に従ったハイフン区切りのようなので、反転を実行して元に戻す。
	Algo::Reverse(Bytes);
	return Bytes;
}
