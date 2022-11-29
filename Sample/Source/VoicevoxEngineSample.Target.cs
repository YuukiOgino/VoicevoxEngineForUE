// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class VoicevoxEngineSampleTarget : TargetRules
{
	public VoicevoxEngineSampleTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "VoicevoxEngineSample" } );
	}
}
