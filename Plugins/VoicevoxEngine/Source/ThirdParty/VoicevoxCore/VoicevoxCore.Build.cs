// Copyright Yuuki Ogino. All Rights Reserved.

using System;
using System.Collections.Generic;
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
			
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, platformName)));
			
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "voicevox_core.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("voicevox_core.dll");
			PublicDelayLoadDLLs.Add("onnxruntime.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/voicevox_core.dll", Path.Combine(ModuleDirectory, platformName, "voicevox_core.dll"));
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/onnxruntime.dll", Path.Combine(ModuleDirectory, platformName, "onnxruntime.dll"));
			
			var ProvidersSharedPath = Path.Combine(ModuleDirectory, platformName, "onnxruntime_providers_shared.dll");
			if (File.Exists(ProvidersSharedPath))
			{
				PublicDelayLoadDLLs.Add("onnxruntime_providers_shared.dll");
				RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/onnxruntime_providers_shared.dll", ProvidersSharedPath);
			}
            
			var CudaPath = Path.Combine(ModuleDirectory, platformName, "onnxruntime_providers_cuda.dll");
			if (File.Exists(CudaPath))
			{
				PublicDelayLoadDLLs.Add("onnxruntime_providers_cuda.dll");
				RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/onnxruntime_providers_cuda.dll", CudaPath);
			}
			
			var TensorrtPath = Path.Combine(ModuleDirectory, platformName, "onnxruntime_providers_tensorrt.dll");
			if (File.Exists(TensorrtPath))
			{
				PublicDelayLoadDLLs.Add("onnxruntime_providers_tensorrt.dll");
				RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/onnxruntime_providers_tensorrt.dll", TensorrtPath);
			}
			
			// Open JTalkライブラリフォルダもコピーする
			AddRuntimeDependenciesDirectory(OpenJtalkDicName, platformName, binPlatformName, true);
			// modelフォルダもコピーする
			AddRuntimeDependenciesDirectory("model", platformName, binPlatformName, true);
        }
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			const string platformName = "osx";
			const string binPlatformName = "Mac";
			
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, platformName)));
			
			// Delay-load the DLL, so we can load it from the right place first
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "libonnxruntime.1.13.1.dylib"));
			
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libonnxruntime.1.13.1.dylib"));
			
			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/libvoicevox_core.dylib", Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/libonnxruntime.1.13.1.dylib", Path.Combine(ModuleDirectory, platformName, "libonnxruntime.1.13.1.dylib"));
			
			// Open JTalkライブラリフォルダもコピーする
			AddRuntimeDependenciesDirectory(OpenJtalkDicName, platformName, binPlatformName, true);
			// modelフォルダもコピーする
			AddRuntimeDependenciesDirectory("model", platformName, binPlatformName, true);
		}
		
		PublicDefinitions.Add($"OPEN_JTALK_DIC_NAME=\"{OpenJtalkDicName}\"");
	}
	
	/// <summary>
	/// プロジェクトディレクトリ名を取得
	/// </summary>
	private static string ProjectDirectory
	{
		get
		{
			foreach (var CommandLineArg in Environment.GetCommandLineArgs())
			{
				var NameAndValues = new List<string>(CommandLineArg.Split('=', StringSplitOptions.RemoveEmptyEntries));
				if (NameAndValues.Count < 2)
				{
					continue;
				}
				if (NameAndValues[0] != "-Project")
				{
					continue;
				}
				NameAndValues.RemoveAt(0);
				var UProjectFilePath = string.Join("", NameAndValues.ToArray());
				return Path.GetDirectoryName(UProjectFilePath);
			}

			return null;
		}
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
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{BinPlatform}/{SourceDirName}/{F.Name}", Path.Combine(ModuleDirectory, Platform, SourceDirName, F.Name));
		}
		
		if (!CopySubDirs) return;
		
		foreach (var SubDir in Infos)
		{
			var TempPath = Path.Combine(SourceDirName, SubDir.Name);
			AddRuntimeDependenciesDirectory(TempPath, Platform, BinPlatform, true);
		}
	}
}
