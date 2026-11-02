# VOICEVOX CORE、Open JTalkをこのフォルダに配置してください。

- 格納例<br/>
<img width="726" alt="スクリーンショット 2024-11-15 2 46 03" src="https://github.com/user-attachments/assets/11ac1bd9-24f6-49c4-9ff6-176cf04c2230">


## CPUモードの場合

[VOICEVOX CORE 0.16.0](https://github.com/VOICEVOX/voicevox_core/releases/tag/0.16.0)から【voicevox_core-osx-arm64-0.16.0.zip】
をダウンロードしてください。<br/>
ダウンロードしたzipファイルは適当なフォルダで解凍をしてください。

また、ターミナルで以下のコマンドを実施すれば、ダウンローダー経由からライブラリを入手できます。

```
curl -sSfL https://github.com/VOICEVOX/voicevox_core/releases/download/0.16.4/download-osx-arm64 -o download
chmod +x download
./download --c-api-version 0.16.0
```

> [!WARNING]
> ダウンローダーの最新版(Latest)はVOICEVOX CORE 0.16.ｘ以上のダウンロードに対応しており、15.9以下のバージョンはフォルダ構造の違いからダウンロードできません。<br/>
> ダウンローダーは必ず16.ｘから使用してください。<br/>
> https://github.com/VOICEVOX/voicevox_core/releases/tag/0.16.0

以下のdylib及びファイルを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* libvoicevox_core.dylib
* libvoicevox_onnxruntime.1.17.3.dylib

## GPUモードの場合

> [!NOTE]
> MacはGPUモード非対応です。
> > macOS の場合、CUDA の macOS サポートは現在終了しているため、VOICEVOX CORE の macOS 向けコアライブラリも CUDA, CUDNN を利用しない CPU 版のみの提供となります。
