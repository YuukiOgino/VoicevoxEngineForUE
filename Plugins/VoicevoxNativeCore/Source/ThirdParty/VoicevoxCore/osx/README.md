# VOICEVOX CORE、Open JTalkをこのフォルダに配置してください。

- 格納例<br/>
<img width="674" height="326" alt="スクリーンショット 2026-11-03 8 54 42" src="https://github.com/user-attachments/assets/fcc43ee4-551e-4920-8c98-e5c1c5388639" />


## CPUモードの場合

[VOICEVOX CORE 0.16.4](https://github.com/VOICEVOX/voicevox_core/releases/tag/0.16.4)から【voicevox_core-osx-arm64-0.16.4.zip】
をダウンロードしてください。<br/>
ダウンロードしたzipファイルは適当なフォルダで解凍をしてください。

また、ターミナルで以下のコマンドを実施すれば、ダウンローダー経由からライブラリを入手できます。

```
curl -sSfL https://github.com/VOICEVOX/voicevox_core/releases/download/0.16.4/download-osx-arm64 -o download
chmod +x download
./download --c-api-version 0.16.4
```

> [!WARNING]
> ダウンローダーの最新版(Latest)はVOICEVOX CORE 0.16.ｘ以上のダウンロードに対応しており、15.9以下のバージョンはフォルダ構造の違いからダウンロードできません。<br/>
> ダウンローダーは必ず16.ｘから使用してください。<br/>
> https://github.com/VOICEVOX/voicevox_core/releases/tag/0.16.4

以下のdylib及びファイルを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* libvoicevox_core.dylib
* libvoicevox_onnxruntime.1.17.3.dylib

## GPUモードの場合

> [!NOTE]
> MacはGPUモード非対応です。
> > macOS の場合、CUDA の macOS サポートは現在終了しているため、VOICEVOX CORE の macOS 向けコアライブラリも CUDA, CUDNN を利用しない CPU 版のみの提供となります。
