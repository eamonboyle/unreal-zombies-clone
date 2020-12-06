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

uint8 AZombieBase::GetPointsForKill(FString BoneName)
{
	if (BoneName.Contains(FString("l")) || BoneName.Contains(FString("r")))
	{
		// limb hit
		UE_LOG(LogTemp, Warning, TEXT("Hit Bone: Limb"));
		return 50;
	}
	else if (BoneName.Contains(FString("spine")) || BoneName.Contains(FString("pelvis")))
	{
		// torso hit
		UE_LOG(LogTemp, Warning, TEXT("Hit Bone: Torso"));
		return 60;
	}
	else if (BoneName.Equals(FString("neck_01")))
	{
		// neck hit
		UE_LOG(LogTemp, Warning, TEXT("Hit Bone: Neck"));
		return 70;
	}
	else if (BoneName.Equals(FString("head")))
	{
		// head hit
		UE_LOG(LogTemp, Warning, TEXT("Hit Bone: Head"));
		return 100;
	}

	return 0;
}

void AZombieBase::Hit(AZombieCharacter* Player, FHitResult HitResult)
{
	// see where the shot hit, calculate points / damage based on the body part
	if (Player != nullptr)
	{
		if (AZombiePlayerState* PState = Player->GetPlayerState<AZombiePlayerState>())
		{
			FString BoneName = HitResult.BoneName.ToString();
			if (BoneName == FString("None"))
			{
				return;
			}
			
			PState->IncrementPoints(GetPointsForKill(BoneName));
		}
	}
}
