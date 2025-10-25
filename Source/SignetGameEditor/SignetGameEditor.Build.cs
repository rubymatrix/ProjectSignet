// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SignetGameEditor : ModuleRules
{
	public SignetGameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(["SignetGameEditor"]);
		
		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "PhysicsCore", "GameplayTags",
			"SignetGame",
		]);
		
		PrivateDependencyModuleNames.AddRange([
			"EditorSubsystem", "EditorScriptingUtilities", "EditorFramework","UnrealEd","Chaos", 
			"Projects", "DeveloperSettings", "DeveloperToolSettings",
			"Slate", "SlateCore", "InputCore", "ToolMenus", "EditorStyle",
			"DataValidation", "MessageLog",
			"CollectionManager", "SourceControl",
		]);
		
		DynamicallyLoadedModuleNames.AddRange(
			[
			]
		);
		
		PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");
	}
}