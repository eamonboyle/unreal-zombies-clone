// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameInstanceBase.h"
#include "Engine/World.h"

UZombieGameInstanceBase::UZombieGameInstanceBase()
{
}

void UZombieGameInstanceBase::GoToMap(FString MAPURL)
{
    GetWorld()->ServerTravel(MAPURL);
}
