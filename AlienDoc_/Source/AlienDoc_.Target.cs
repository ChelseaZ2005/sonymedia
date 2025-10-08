using UnrealBuildTool;
using System.Collections.Generic;

public class AlienDoc__Target : TargetRules
{
	public AlienDoc__Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.AddRange(new string[] { "AlienDocGame", "AlienDocGeminiVA" });
	}
}


