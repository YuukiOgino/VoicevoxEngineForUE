// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class VoicevoxNativeNemoCore : ModuleRules
{
	/// <summary>
	/// Open JTalkのライブラリフォルダ名
	/// </summary>
	private const string OpenJtalkDicName = "open_jtalk_dic_utf_8-1.11";
	
	public VoicevoxNativeNemoCore(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		CppStandard = CppStandardVersion.Latest;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			const string platformName = "x64";
			const string binPlatformName = "Win64";
			const string thirdPartyName = "VoicevoxCoreNemo";
			
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, platformName)));
			
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "voicevox_core.lib"));
			
			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("voicevox_core.dll");
			PublicDelayLoadDLLs.Add("onnxruntime.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/voicevox_core.dll", Path.Combine(ModuleDirectory, platformName, "voicevox_core.dll"));
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/onnxruntime.dll", Path.Combine(ModuleDirectory, platformName, "onnxruntime.dll"));
			
			// onnxruntimeのCUDE関連DLLが存在する場合は必要なDLL一式すべてコピーする
			var CudaPath = Path.Combine(ModuleDirectory, platformName, "onnxruntime_providers_cuda.dll");
			if (File.Exists(CudaPath))
			{
				var CudaDllList = new[]
				{
					"onnxruntime_providers_cuda.dll",
					"onnxruntime_providers_shared.dll",
					"onnxruntime_providers_tensorrt.dll",
					"cublas64_11.dll",
					"cublasLt64_11.dll",
					"cudart64_110.dll",
					"cudnn_adv_infer64_8.dll",
					"cudnn_cnn_infer64_8.dll",
					"cudnn_ops_infer64_8.dll",
					"cudnn64_8.dll",
					"cufft64_10.dll",
					"curand64_10.dll"
				};
				foreach (var Variable in CudaDllList)
				{
					var CudaDllPath = Path.Combine(ModuleDirectory, platformName, Variable) ?? throw new ArgumentNullException("Path.Combine(ModuleDirectory, platformName, Variable)");
					if (!File.Exists(CudaDllPath)) continue;
					PublicDelayLoadDLLs.Add(Variable);
					RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/{Variable}", CudaDllPath);
				}
			}
			
			// DirectML.dllが存在する場合はコピーする
			var DirectMlPath = Path.Combine(ModuleDirectory, platformName, "DirectML.dll");
			if (File.Exists(DirectMlPath))
			{
				PublicDelayLoadDLLs.Add("DirectML.dll");
				RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/DirectML.dll", DirectMlPath);
			}
			
			// modelフォルダもコピーする
			AddRuntimeDependenciesDirectory("model", platformName, binPlatformName, true);
        }
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			const string platformName = "osx";
			const string binPlatformName = "Mac";
			const string thirdPartyName = "VoicevoxCoreNemo";
			
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, platformName)));
			
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libonnxruntime.1.13.1.dylib"));
			
			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/libvoicevox_core.dylib", Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/libonnxruntime.1.13.1.dylib", Path.Combine(ModuleDirectory, platformName, "libonnxruntime.1.13.1.dylib"));
			
			// modelフォルダもコピーする
			AddRuntimeDependenciesDirectory("model", platformName, binPlatformName, true);
		}
		
		PublicDefinitions.Add($"OPEN_JTALK_DIC_NAME=\"{OpenJtalkDicName}\"");
	}

	/// <summary>
	/// 指定したディレクトリをRuntimeDependenciesに登録
	/// </summary>
	/// <param name="SourceDirName">コピー元のディレクトリ名</param>
	/// <param name="Platform">プラグイン側のプラットフォームフォルダ名</param>
	/// <param name="BinPlatform">出力バイナリのプラットフォームフォルダ名</param>
	/// <param name="CopySubDirs">サブディレクトリもコピーを行うか</param>
	/// <exception cref="DirectoryNotFoundException"></exception>
	private void AddRuntimeDependenciesDirectory(string SourceDirName, string Platform, string BinPlatform,  bool CopySubDirs)
	{
		var Info = new DirectoryInfo(Path.Combine(ModuleDirectory, Platform, SourceDirName));
		if (!Info.Exists)
		{
			throw new DirectoryNotFoundException($"The specified directory cannot be found: {SourceDirName}");
		}
		
		var Infos = Info.GetDirectories();
		var Files = Info.GetFiles();
		
		foreach (var F in Files)
		{
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/VoicevoxCoreNemo/{BinPlatform}/{SourceDirName}/{F.Name}", Path.Combine(ModuleDirectory, Platform, SourceDirName, F.Name));
		}
		
		if (!CopySubDirs) return;
		
		foreach (var SubDir in Infos)
		{
			var TempPath = Path.Combine(SourceDirName, SubDir.Name);
			AddRuntimeDependenciesDirectory(TempPath, Platform, BinPlatform, true);
		}
	}
}
