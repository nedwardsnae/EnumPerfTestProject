// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class EnumPerfTestProjectEditorTarget : TargetRules
{
	public EnumPerfTestProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "EnumPerfTestProject" } );
	}
}
