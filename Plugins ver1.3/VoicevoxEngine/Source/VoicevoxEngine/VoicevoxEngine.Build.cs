// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;

/// <summary>
/// VoicevoxEngine ModuleRules
/// </summary>
public class VoicevoxEngine : ModuleRules
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="target"></param>
	public VoicevoxEngine(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Latest;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"Engine",
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"VoicevoxUECore",
				"Json",
				"JsonUtilities"
			});
	}
}
