// Copyright Yuuki Ogino. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

/// <summary>
/// VOICEVOX COREライブラリモジュールクラス
/// </summary>
public class VoicevoxCore : ModuleRules
{
	/// <summary>
	/// Open JTalkのライブラリフォルダ名
	/// </summary>
	private const string OpenJtalkDicName = "open_jtalk_dic_utf_8-1.11";

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="Target"></param>
	public VoicevoxCore(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		CppStandard = CppStandardVersion.Latest;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			const string platformName = "x64";
			const string binPlatformName = "Win64";
			const string thirdPartyName = "VoicevoxCore";
			
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "voicevox_core.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "voicevox_onnxruntime.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("voicevox_core.dll");
			PublicDelayLoadDLLs.Add("voicevox_onnxruntime.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/voicevox_core.dll", Path.Combine(ModuleDirectory, platformName, "voicevox_core.dll"));
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/voicevox_onnxruntime.dll", Path.Combine(ModuleDirectory, platformName, "voicevox_onnxruntime.dll"));

			var providerSharedLibPath = Path.Combine(ModuleDirectory, platformName, "voicevox_onnxruntime_providers_shared.lib");
			if (File.Exists(providerSharedLibPath))
			{
				PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "voicevox_onnxruntime_providers_shared.lib"));
			}
			
			var providerSharedPath = Path.Combine(ModuleDirectory, platformName, "voicevox_onnxruntime_providers_shared.dll");
			if (File.Exists(providerSharedPath))
			{
				PublicDelayLoadDLLs.Add("voicevox_onnxruntime_providers_shared.dll");
				RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/voicevox_onnxruntime_providers_shared.dll", providerSharedPath);
			}
			
			// onnxruntimeのCUDE関連DLLが存在する場合は必要なDLL一式すべてコピーする
			var cudaPath = Path.Combine(ModuleDirectory, platformName, "voicevox_onnxruntime_providers_cuda.dll");
			if (File.Exists(cudaPath))
			{
				// VOICEVOX CORE 16からlibが追加
				PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "voicevox_onnxruntime_providers_cuda.lib"));
				PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "zlibwapi.lib"));
				
				var cudaDllList = new[]
				{
					"voicevox_onnxruntime_providers_cuda.dll",
					"cublas64_12.dll",
					"cublasLt64_12.dll",
					"cudart64_12.dll",
					"cudnn_adv_infer64_8.dll",
					"cudnn_cnn_infer64_8.dll",
					"cudnn_ops_infer64_8.dll",
					"cudnn64_8.dll",
					"cufft64_11.dll",
					"curand64_10.dll",
					"zlibwapi.dll"
				};
				foreach (var variable in cudaDllList)
				{
					var cudaLibPath = Path.Combine(ModuleDirectory, platformName, variable);
					if (!File.Exists(cudaLibPath)) continue;
					PublicDelayLoadDLLs.Add(variable);
					RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/{variable}", cudaLibPath);
				}
			}
			
			// DirectML.dllが存在する場合はコピーする
			var directMlPath = Path.Combine(ModuleDirectory, platformName, "DirectML.dll");
			if (File.Exists(directMlPath))
			{
				PublicDelayLoadDLLs.Add("DirectML.dll");
				RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/DirectML.dll", directMlPath);
			}
			
			// Open JTalkライブラリフォルダもコピーする
			AddRuntimeDependenciesDirectory(OpenJtalkDicName, platformName, binPlatformName, true);
			// modelフォルダもコピーする
			AddRuntimeDependenciesThirdPartyDirectory("models", platformName, binPlatformName, true);
        }
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			const string platformName = "osx";
			const string binPlatformName = "Mac";
			const string thirdPartyName = "VoicevoxCore";
			
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libvoicevox_onnxruntime.1.17.3.dylib"));
			
			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/libvoicevox_core.dylib", Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/llibvoicevox_onnxruntime.1.17.3.dylib", Path.Combine(ModuleDirectory, platformName, "libvoicevox_onnxruntime.1.17.3.dylib"));
			
			// Open JTalkライブラリフォルダもコピーする
			AddRuntimeDependenciesDirectory(OpenJtalkDicName, platformName, binPlatformName, true);
			// modelフォルダもコピーする
			AddRuntimeDependenciesThirdPartyDirectory("models", platformName, binPlatformName, true);
		}
		
		PublicDefinitions.Add($"OPEN_JTALK_DIC_NAME=\"{OpenJtalkDicName}\"");
	}

	/// <summary>
	/// 指定したディレクトリをRuntimeDependenciesに登録
	/// </summary>
	/// <param name="sourceDirName">コピー元のディレクトリ名</param>
	/// <param name="platform">プラグイン側のプラットフォームフォルダ名</param>
	/// <param name="binPlatform">出力バイナリのプラットフォームフォルダ名</param>
	/// <param name="copySubDirs">サブディレクトリもコピーを行うか</param>
	/// <exception cref="DirectoryNotFoundException"></exception>
	private void AddRuntimeDependenciesDirectory(string sourceDirName, string platform, string binPlatform,  bool copySubDirs)
	{
		var info = new DirectoryInfo(Path.Combine(ModuleDirectory, platform, sourceDirName));
		if (!info.Exists)
		{
			throw new DirectoryNotFoundException($"The specified directory cannot be found: {sourceDirName}");
		}
		
		var infos = info.GetDirectories();
		var files = info.GetFiles();
		
		foreach (var f in files)
		{
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatform}/{sourceDirName}/{f.Name}", Path.Combine(ModuleDirectory, platform, sourceDirName, f.Name));
		}
		
		if (!copySubDirs) return;
		
		foreach (var subDir in infos)
		{
			var tempPath = Path.Combine(sourceDirName, subDir.Name);
			AddRuntimeDependenciesDirectory(tempPath, platform, binPlatform, true);
		}
	}
	
	/// <summary>
	/// 指定したディレクトリをRuntimeDependenciesに登録
	/// </summary>
	/// <param name="sourceDirName">コピー元のディレクトリ名</param>
	/// <param name="platform">プラグイン側のプラットフォームフォルダ名</param>
	/// <param name="binPlatform">出力バイナリのプラットフォームフォルダ名</param>
	/// <param name="copySubDirs">サブディレクトリもコピーを行うか</param>
	/// <exception cref="DirectoryNotFoundException"></exception>
	private void AddRuntimeDependenciesThirdPartyDirectory(string sourceDirName, string platform, string binPlatform,  bool copySubDirs)
	{
		var info = new DirectoryInfo(Path.Combine(ModuleDirectory, platform, sourceDirName));
		if (!info.Exists)
		{
			throw new DirectoryNotFoundException($"The specified directory cannot be found: {sourceDirName}");
		}
		
		var infos = info.GetDirectories();
		var files = info.GetFiles();
		
		foreach (var f in files)
		{
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/VoicevoxCore/{binPlatform}/{sourceDirName}/{f.Name}", Path.Combine(ModuleDirectory, platform, sourceDirName, f.Name));
		}
		
		if (!copySubDirs) return;
		
		foreach (var subDir in infos)
		{
			var tempPath = Path.Combine(sourceDirName, subDir.Name);
			AddRuntimeDependenciesDirectory(tempPath, platform, binPlatform, true);
		}
	}
}
