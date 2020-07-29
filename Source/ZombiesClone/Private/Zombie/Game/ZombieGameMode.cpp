// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameMode.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "Zombie/Game/ZombieSpawnPoint.h"
#include "ZombiesClone/ZombiesCloneHUD.h"
#include "ZombiesClone/Public/Player/ZombieCharacter.h"
#include "ZombiesClone/Public/Zombie/Enemy/ZombieBase.h"
#include "ZombiesClone/Public/Zombie/Game/ZombieGameState.h"

AZombieGameMode::AZombieGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
        TEXT("/Game/Blueprints/Player/BP_ZombieCharacter.BP_ZombieCharacter_C"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    HUDClass = AZombiesCloneHUD::StaticClass();

    bHasLoadedSpawnPoints = false;

    ZombiesRemaining = 0;
}

void AZombieGameMode::BeginPlay()
{
    Super::BeginPlay();

    ZombieGameState = GetGameState<AZombieGameState>();
    CalculateZombieCount();

    // get all of the zombie spawn points in the game
    TArray<AActor*> TempActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombieSpawnPoint::StaticClass(), TempActors);

    if (TempActors.Num() > 0)
    {
        for (AActor* TempActor : TempActors)
        {
            if (AZombieSpawnPoint* ZombieSpawnPoint = Cast<AZombieSpawnPoint>(TempActor))
            {
                ZombieSpawnPoints.Add(ZombieSpawnPoint);
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Zombie Spawn Points: %d"), ZombieSpawnPoints.Num());

    // start the zombie spawn timer
    GetWorld()->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &AZombieGameMode::SpawnZombie, 2.0f, true);
}

void AZombieGameMode::CalculateZombieCount()
{
    if (ZombieGameState != nullptr)
    {
        uint16 RoundNumber = ZombieGameState->GetRoundNumber();

        // do calculations to get the Zombie amount here
        ZombiesRemaining = 5;
    }
}

void AZombieGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!bHasLoadedSpawnPoints)
    {
        SetPlayerSpawnPoints();
    }

    if (PlayerSpawnPoints.Num() > 0)
    {
        for (AZombiePlayerSpawnPoint* PlayerSpawnPoint : PlayerSpawnPoints)
        {
            if (!PlayerSpawnPoint->IsUsed())
            {
                FVector SpawnLocation = PlayerSpawnPoint->GetActorLocation();

                if (APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(PlayerClass, SpawnLocation,
                                                                      FRotator::ZeroRotator))
                {
                    NewPlayer->Possess(PlayerPawn);
                    PlayerSpawnPoint->SetUsed(true);
                    return;
                }
            }
        }
    }
}

void AZombieGameMode::SetPlayerSpawnPoints()
{
    // get all of the player spawn points in the game
    TArray<AActor*> TempActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombiePlayerSpawnPoint::StaticClass(), TempActors);

    if (TempActors.Num())
    {
        for (AActor* TempActor : TempActors)
        {
            if (AZombiePlayerSpawnPoint* PlayerSpawnPoint = Cast<AZombiePlayerSpawnPoint>(TempActor))
            {
                PlayerSpawnPoints.Add(PlayerSpawnPoint);
            }
        }
    }

    bHasLoadedSpawnPoints = true;
}

void AZombieGameMode::SpawnZombie()
{
    if (ZombiesRemaining > 0)
    {
        int RandomIndex = FMath::RandRange(0, ZombieSpawnPoints.Num() - 1);

        if (ZombieSpawnPoints.IsValidIndex(RandomIndex))
        {
            if (AZombieSpawnPoint* SpawnPoint = ZombieSpawnPoints[RandomIndex])
            {
                FVector Location = SpawnPoint->GetActorLocation();
                FRotator Rotation = SpawnPoint->GetActorRotation();

                if (AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Location, Rotation))
                {
                    UE_LOG(LogTemp, Warning, TEXT("SPAWNING ZOMBIE"));
                    --ZombiesRemaining;
                }
            }
        }
    }
    else
    {
        GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
    }
}
