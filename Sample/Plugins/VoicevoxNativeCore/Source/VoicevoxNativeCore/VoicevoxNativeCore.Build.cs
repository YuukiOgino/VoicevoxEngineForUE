// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxNativeCore : ModuleRules
{
	public VoicevoxNativeCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Latest;
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine"
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"VoicevoxCore",
				"VoicevoxUECore",　// 不要になるかも？
				"Json",
				"JsonUtilities"
			});
	}
}
