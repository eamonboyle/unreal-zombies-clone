// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieSpawnPoint.h"
#include "ZombiesClone/Public/Zombie/Useables/Barricade.h"

AZombieSpawnPoint::AZombieSpawnPoint()
{
    Zone = 0;
}

void AZombieSpawnPoint::BeginPlay()
{
    Super::BeginPlay();
}

ABarricade* AZombieSpawnPoint::GetLinkedBarricade()
{
    return LinkedBarricade;
}

uint8 AZombieSpawnPoint::GetZone()
{
    return Zone;
}

void AZombieSpawnPoint::SetZone(uint8 ZoneNumber)
{
    Zone = ZoneNumber;
}
