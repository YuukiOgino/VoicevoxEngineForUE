// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxNativeCoreNemo : ModuleRules
{
	public VoicevoxNativeCoreNemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Latest;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"CoreUObject",
				"VoicevoxCoreNemo",
				"VoicevoxUECore"
			}
		);
		
		PrivateDefinitions.Add($"VOICEVOX_NEMO_CORE_NAME=\"NEMO CORE\"");
	}
}
