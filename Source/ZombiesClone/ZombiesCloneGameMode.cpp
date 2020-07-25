// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombiesCloneGameMode.h"
#include "ZombiesCloneHUD.h"
#include "ZombiesClone/Public/Player/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"

AZombiesCloneGameMode::AZombiesCloneGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AZombiesCloneHUD::StaticClass();
}
