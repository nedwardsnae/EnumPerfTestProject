using UnrealBuildTool;

public class EnumPerfTestProject : ModuleRules
{
	public EnumPerfTestProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage            = PCHUsageMode.UseExplicitOrSharedPCHs;
		bFasterWithoutUnity = true;

		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
	}
}
