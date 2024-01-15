// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EchoesOfAcropolis : ModuleRules
{
	public EchoesOfAcropolis(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NinjaCharacter", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}
