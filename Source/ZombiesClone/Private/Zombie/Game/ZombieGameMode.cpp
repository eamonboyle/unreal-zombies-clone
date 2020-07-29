// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameMode.h"

#include "ZombiesClone/ZombiesCloneHUD.h"
#include "ZombiesClone/Public/Player/CharacterBase.h"

AZombieGameMode::AZombieGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase.BP_CharacterBase_C"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    HUDClass = AZombiesCloneHUD::StaticClass();
}
