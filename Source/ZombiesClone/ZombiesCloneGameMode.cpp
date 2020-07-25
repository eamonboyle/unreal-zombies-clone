// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombiesCloneGameMode.h"
#include "ZombiesCloneHUD.h"
#include "ZombiesCloneCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZombiesCloneGameMode::AZombiesCloneGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AZombiesCloneHUD::StaticClass();
}
