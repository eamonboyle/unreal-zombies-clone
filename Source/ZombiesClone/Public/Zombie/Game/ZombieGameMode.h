// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombiePlayerSpawnPoint.h"

#include "ZombieGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESCLONE_API AZombieGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AZombieGameMode();

protected:
    class AZombieGameState* ZombieGameState;
    
    bool bHasLoadedSpawnPoints;

    UPROPERTY()
    TArray<class AZombiePlayerSpawnPoint*> PlayerSpawnPoints;

    UPROPERTY()
    TArray<class AZombieSpawnPoint*> ZombieSpawnPoints;

    UPROPERTY(EditAnywhere, Category = "ZombieSettings")
    TSubclassOf<class AZombieCharacter> PlayerClass;

    UPROPERTY(EditAnywhere, Category = "ZombieSettings")
    TSubclassOf<class AZombieBase> ZombieClass;

    FTimerHandle TZombieSpawnHandle;

    uint16 ZombiesRemaining;

protected:
    void CalculateZombieCount();
    void SpawnZombie();

protected:
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

    void SetPlayerSpawnPoints();
};
