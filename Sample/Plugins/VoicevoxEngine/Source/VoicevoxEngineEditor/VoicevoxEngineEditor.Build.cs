using UnrealBuildTool;

public class VoicevoxEngineEditor : ModuleRules
{
    public VoicevoxEngineEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Blutility",
                "UMG",
                "UMGEditor"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "VoicevoxEngine",
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}