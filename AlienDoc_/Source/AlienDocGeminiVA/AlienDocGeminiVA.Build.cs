using UnrealBuildTool;

public class AlienDocGeminiVA : ModuleRules
{
	public AlienDocGeminiVA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"HTTP",
			"Json",
			"JsonUtilities"
		});
	}
}


