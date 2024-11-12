// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class VoicevoxCoreNemo : ModuleRules
{
	/// <summary>
	/// Open JTalkのライブラリフォルダ名
	/// </summary>
	private const string OpenJtalkDicName = "open_jtalk_dic_utf_8-1.11";
	
	public VoicevoxCoreNemo(ReadOnlyTargetRules Target) : base(Target)
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
			
			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/voicevox_core.dll", Path.Combine(ModuleDirectory, platformName, "voicevox_core.dll"));
			
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
			
			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/{thirdPartyName}/{binPlatformName}/libvoicevox_core.dylib", Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			
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
		var info = new DirectoryInfo(Path.Combine(ModuleDirectory, Platform, SourceDirName));
		if (!info.Exists)
		{
			throw new DirectoryNotFoundException($"The specified directory cannot be found: {SourceDirName}");
		}
		
		var infos = info.GetDirectories();
		var files = info.GetFiles();
		
		foreach (var f in files)
		{
			RuntimeDependencies.Add($"$(PluginDir)/Binaries/ThirdParty/VoicevoxCoreNemo/{BinPlatform}/{SourceDirName}/{f.Name}", Path.Combine(ModuleDirectory, Platform, SourceDirName, f.Name));
		}
		
		if (!CopySubDirs) return;
		
		foreach (var subDir in infos)
		{
			var tempPath = Path.Combine(SourceDirName, subDir.Name);
			AddRuntimeDependenciesDirectory(tempPath, Platform, BinPlatform, true);
		}
	}
}
