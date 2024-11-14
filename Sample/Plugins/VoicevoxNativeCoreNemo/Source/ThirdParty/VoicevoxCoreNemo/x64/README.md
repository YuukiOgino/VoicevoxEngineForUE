# VOICEVOX COREをこのフォルダに配置してください。

- 格納例<br/>
![image](https://github.com/user-attachments/assets/43787d70-d804-489a-b705-8ca5959b6843)

> [!NOTE]
> Open JTalk、ONNX RuntimeはVoicevoxNativeCoreプラグイン側で行います。このフォルダに配置する必要はありません。

## 注意点

VoicevoxNativeCoreプラグインに配置したvoicevox_coreライブラリがGPUモードの場合、VOICEVOX NEMOのvoicevox_coreライブラリも**必ずGPUモードのDLLとlibファイルを配置してください**。<br/>
VoicevoxNativeCoreプラグインが **GPUモード対応ライブラリ**  、VoicevoxNativeCoreNemoプラグインに配置したvoicevox_coreライブラリが**CPUモード対応ライブラリ**の場合は**初期化に失敗して動作しません**。逆も同じです。

## CPUモードの場合

* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib

## GPUモードの場合（DirectML、CUDA）

> [!NOTE]
> GPU起動に必要なDLLはダウンローダー以外では入手できません。必ずダウンローダーからVOICEVOX COREを入手してください。

ダウンローダー　DirectML版コマンド例
```PowerShell
Invoke-WebRequest https://github.com/VOICEVOX/voicevox_core/releases/latest/download/download-windows-x64.exe -OutFile ./download.exe
./download.exe -v 0.14.6 --device directml
```

* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib
