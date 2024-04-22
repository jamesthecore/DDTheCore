// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DDTheCore : ModuleRules
{
	public DDTheCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bLegacyPublicIncludePaths = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils" , "OnlineSubsystemSteam"});
		PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });
	}
}
