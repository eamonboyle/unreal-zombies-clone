// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameMode.h"


#include "Kismet/GameplayStatics.h"
#include "ZombiesClone/ZombiesCloneHUD.h"
#include "ZombiesClone/Public/Player/CharacterBase.h"

AZombieGameMode::AZombieGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
        TEXT("/Game/Blueprints/Player/BP_CharacterBase.BP_CharacterBase_C"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    HUDClass = AZombiesCloneHUD::StaticClass();

    bHasLoadedSpawnPoints = false;
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

                if (APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation,
                                                                      FRotator::ZeroRotator))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Spawned pawn to possess"));
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

    UE_LOG(LogTemp, Warning, TEXT("Spawn Point Count: %d"), PlayerSpawnPoints.Num());
    bHasLoadedSpawnPoints = true;
}
