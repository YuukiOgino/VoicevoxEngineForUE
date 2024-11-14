# VOICEVOX COREをこのフォルダに配置してください。

- 格納例<br/>
![image](https://github.com/YuukiOgino/VoicevoxEngineForUE/assets/10606646/230a161a-9ed4-47b2-977f-018bcffaf119)

> [!NOTE]
> Open JTalk、ONNX RuntimeはVoicevoxNativeCoreプラグイン側で行います。このフォルダに配置する必要はありません。

## CPUモードの場合

以下のdylib及びファイルを格納してください。

* modelフォルダ
* libonnxruntime.1.13.1.dylib

## GPUモードの場合

> [!NOTE]
> MacはGPUモード非対応です。
> > macOS の場合、CUDA の macOS サポートは現在終了しているため、VOICEVOX CORE の macOS 向けコアライブラリも CUDA, CUDNN を利用しない CPU 版のみの提供となります。
