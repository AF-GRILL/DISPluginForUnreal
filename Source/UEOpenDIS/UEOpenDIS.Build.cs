// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UEOpenDIS : ModuleRules
{
	public UEOpenDIS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"CoreUObject",
				"Engine",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
			);

        var BasePath = Path.GetDirectoryName(RulesCompiler.GetFileNameFromType(GetType()));
        string ThirdPartyPath = Path.Combine(BasePath, "..", "..", "ThirdParty");
        string BinaryPath = Path.Combine(ThirdPartyPath, "Binaries");
        PublicLibraryPaths.Add(Path.Combine(BinaryPath, "Win64"));
        PublicAdditionalLibraries.Add("OpenDIS6.lib");
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
	}
}
