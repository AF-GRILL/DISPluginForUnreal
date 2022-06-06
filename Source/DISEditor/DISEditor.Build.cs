// Copyright 2022 Gaming Research Integration for Learning Lab. All Rights Reserved.

using UnrealBuildTool;

public class DISEditor : ModuleRules
{
	public DISEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "DISRuntime",
                "AssetTools"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Projects",
                "InputCore",
                "UnrealEd",
                "ToolMenus",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
            );
    }
}
