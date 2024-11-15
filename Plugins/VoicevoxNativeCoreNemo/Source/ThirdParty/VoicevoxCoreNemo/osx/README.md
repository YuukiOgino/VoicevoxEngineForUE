# VOICEVOX COREをこのフォルダに配置してください。

- 格納例<br/>
<img width="730" alt="スクリーンショット 2024-11-15 2 56 16" src="https://github.com/user-attachments/assets/13aa856a-8bc9-452d-b053-0aa9332a3f4e">

> [!NOTE]
> Open JTalk、ONNX RuntimeはVoicevoxNativeCoreプラグイン側で行います。このフォルダに配置する必要はありません。

## CPUモードの場合

(VOICEVOX NEMO CORE 0.14.0)[https://github.com/VOICEVOX/voicevox_nemo_core/releases/tag/0.14.0]から【voicevox_core-osx-arm64-cpu-0.14.0.zip】
をダウンロードしてください。<br/>
ダウンロードしたzipファイルは適当なフォルダで解凍をしてください。

また、ターミナルで以下のコマンドを実施すれば、ダウンローダー経由からライブラリを入手できます。

```
curl -sSfL https://github.com/VOICEVOX/voicevox_nemo_core/releases/latest/download/download-osx-arm64 -o download
chmod +x download
./download -v 0.14.0
```

以下のdylib及びフォルダを格納してください。

* modelフォルダ
* libonnxruntime.1.13.1.dylib

## GPUモードの場合

> [!NOTE]
> MacはGPUモード非対応です。
> > macOS の場合、CUDA の macOS サポートは現在終了しているため、VOICEVOX CORE の macOS 向けコアライブラリも CUDA, CUDNN を利用しない CPU 版のみの提供となります。
