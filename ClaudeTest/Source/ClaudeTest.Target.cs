using UnrealBuildTool;
using System.Collections.Generic;

public class ClaudeTestTarget : TargetRules
{
	public ClaudeTestTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ClaudeTest" } );
	}
}