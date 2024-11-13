// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxEngineEditor : ModuleRules
{
    public VoicevoxEngineEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "Blutility",
                "UMG",
                "UMGEditor"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                "VoicevoxUECore",
                "VoicevoxUECoreEditor",
                "VoicevoxNativeCore", //　参照用のEngineSybsystemが完成したら消す
                "VoicevoxEngine",
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DesktopPlatform",
                "Json",
                "JsonUtilities"                
            }
        );
    }
}