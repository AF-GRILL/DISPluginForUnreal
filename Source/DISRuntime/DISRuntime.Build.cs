using UnrealBuildTool;
using System.IO;

public class DISRuntime : ModuleRules
{
	private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/")); }
    }
		
	public DISRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bEnableExceptions = true;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Sockets",
				"Networking",
				"GeoReferencing"
			}
			);			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"Slate",
				"CoreUObject",
				"Engine",
				"SlateCore"
			}
			);

		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
		PublicDefinitions.Add("OPENDIS6_STATIC_DEFINE=1");
		PublicDefinitions.Add("OPENDIS7_STATIC_DEFINE=1");
		PublicDefinitions.Add("GRILL_DIS_SUPPORTS_WIN64=1");
		PublicDefinitions.Add("GRILL_DIS_SUPPORTS_MAC=1");

		if (Target.Platform != UnrealTargetPlatform.Win64 && Target.Platform != UnrealTargetPlatform.Mac)
		{
			throw new BuildException("GRILL DIS for Unreal currently supports Win64 and Mac.");
		}
	}
}
