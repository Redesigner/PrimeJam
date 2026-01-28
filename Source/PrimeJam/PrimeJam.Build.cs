// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PrimeJam : ModuleRules
{
	public PrimeJam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"ModelViewViewModel",
			"AIModule",
			"GameplayStateTreeModule",
			"StateTreeModule",
			"GameplayTags",
			"GameplayTasks",
			"NavigationSystem",
			"PropertyBindingUtils"
		]);

		PrivateDependencyModuleNames.AddRange([
		]);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
