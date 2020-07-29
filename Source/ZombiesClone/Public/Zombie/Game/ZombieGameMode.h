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
    bool bHasLoadedSpawnPoints;

    UPROPERTY()
    TArray<class AZombiePlayerSpawnPoint*> PlayerSpawnPoints;

protected:
    virtual void PostLogin(APlayerController* NewPlayer) override;

    void SetPlayerSpawnPoints();
};
