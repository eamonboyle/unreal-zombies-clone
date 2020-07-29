// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameMode.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "Zombie/Game/ZombieSpawnPoint.h"
#include "ZombiesClone/ZombiesCloneHUD.h"
#include "ZombiesClone/Public/Player/ZombieCharacter.h"
#include "ZombiesClone/Public/Zombie/Enemy/ZombieBase.h"
#include "ZombiesClone/Public/Zombie/Game/ZombieGameState.h"
#include "ZombiesClone/Public/Zombie/Useables/Barricade.h"

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
                // sets up the zombies spawn zones
                if (ABarricade* LinkedBarricade = ZombieSpawnPoint->GetLinkedBarricade())
                {
                    ZombieSpawnPoint->SetZone(LinkedBarricade->GetAccessZone());
                    ZombieSpawnPoints.Add(ZombieSpawnPoint);
                    // UE_LOG(LogTemp, Warning, TEXT("Zone Number: %d"), LinkedBarricade->GetAccessZone());
                }
                else
                {
                    ActiveZombieSpawnPoints.Add(ZombieSpawnPoint);
                }
            }
        }
    }

    // UE_LOG(LogTemp, Warning, TEXT("Zombie Spawn Points: %d"), ZombieSpawnPoints.Num());

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

void AZombieGameMode::NewZoneActive(uint8 ZoneNumber)
{
    UE_LOG(LogTemp, Warning, TEXT("Setting active zone: %d"), ZoneNumber);

    int Control = 0;

    for (int16 x = ZombieSpawnPoints.Num() - 1; x >= 0; --x)
    {
        AZombieSpawnPoint* SpawnPoint = ZombieSpawnPoints[x];
        
        UE_LOG(LogTemp, Warning, TEXT("Loop Count: %d"), Control);
        ++Control;
        
        if (SpawnPoint != nullptr && ZoneNumber == SpawnPoint->GetZone() && !SpawnPoint->IsActive())
        {
            ActiveZombieSpawnPoints.Add(SpawnPoint);
            SpawnPoint->Activate();

            // remove spawn point from the ZombieSpawnPoints array
            ZombieSpawnPoints.RemoveAt(x);
        }
    }
}

void AZombieGameMode::SpawnZombie()
{
    if (ZombiesRemaining > 0)
    {
        int RandomIndex = FMath::RandRange(0, ActiveZombieSpawnPoints.Num() - 1);

        if (ActiveZombieSpawnPoints.IsValidIndex(RandomIndex))
        {
            if (AZombieSpawnPoint* SpawnPoint = ActiveZombieSpawnPoints[RandomIndex])
            {
                FVector Location = SpawnPoint->GetActorLocation();
                FRotator Rotation = SpawnPoint->GetActorRotation();

                if (AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Location, Rotation))
                {
                    // UE_LOG(LogTemp, Warning, TEXT("SPAWNING ZOMBIE"));
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
