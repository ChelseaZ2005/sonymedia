using UnrealBuildTool;

public class AlienDocGame : ModuleRules
{
	public AlienDocGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine"
		});
	}
}


