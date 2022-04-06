// Copyright Epic Games, Inc. All Rights Reserved.

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

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Sockets",
				"Networking",
				"GeoReferencing",
				"GeometricObjects"
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
		
        string BinaryPath = Path.Combine(ThirdPartyPath, "Binaries");
		string WinPath = Path.Combine(BinaryPath, "Win64");
        PublicSystemLibraryPaths.Add(WinPath);
        PublicAdditionalLibraries.Add(Path.Combine(WinPath, "OpenDIS6.lib"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
		
		PublicDelayLoadDLLs.Add("OpenDIS6.dll");
		RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(WinPath, "OpenDIS6.dll")));
	}
}
