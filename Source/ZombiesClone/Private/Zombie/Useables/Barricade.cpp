// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Useables/Barricade.h"
#include "ZombiesClone/Public/Zombie/Game/ZombieGameMode.h"
#include "ZombiesClone/Public/Player/ZombieCharacter.h"
#include "ZombiesClone/Public/Player/ZombiePlayerState.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABarricade::ABarricade()
{
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    RootComponent = MeshComp;
    CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

    Cost = 500;
    UIMessage += FString(" [Cost: " + FString::FromInt(Cost) + "]");
    bIsUsed = false;
    AccessZone = 0;
}

void ABarricade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABarricade, bIsUsed);
}

void ABarricade::OnRep_BarricadeUsed()
{
    // SetActorEnableCollision(false);

    if (OpenAnimation != nullptr)
    {
        CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshComp->PlayAnimation(OpenAnimation, false);
    }

    UIMessage = FString();
}

void ABarricade::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
}

void ABarricade::Use(AZombieCharacter* Player)
{
    if (HasAuthority() && !bIsUsed && Player != nullptr)
    {
        if (AZombiePlayerState* PlayerState = Player->GetPlayerState<AZombiePlayerState>())
        {
            // don't allow the player to purchase the barricade if it doesn't have enough points
            if (!PlayerState->DecrementPoints(Cost))
            {
                return;
            }
            
            UE_LOG(LogTemp, Warning, TEXT("USING %s"), *ObjectName);

            // play animation to move barricade
            bIsUsed = true;
            OnRep_BarricadeUsed();

            if (AZombieGameMode* GM = GetWorld()->GetAuthGameMode<AZombieGameMode>())
            {
                GM->NewZoneActive(AccessZone);
            }
        }
    }
}

uint8 ABarricade::GetAccessZone()
{
    return AccessZone;
}
