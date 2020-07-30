// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Enemy/ZombieBase.h"
#include "ZombiesClone/Public/Player/ZombiePlayerState.h"

#include "Player/ZombieCharacter.h"

// Sets default values
AZombieBase::AZombieBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();	
}

void AZombieBase::Hit(AZombieCharacter* Player)
{
	// see where the shot hit, calculate points / damage based on the body part
	if (Player != nullptr)
	{
		if (AZombiePlayerState* PState = Player->GetPlayerState<AZombiePlayerState>())
		{
			PState->IncrementPoints(100);
		}
	}
}
