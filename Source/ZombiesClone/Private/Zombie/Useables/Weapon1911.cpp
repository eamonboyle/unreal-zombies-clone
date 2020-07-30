// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Useables/Weapon1911.h"
#include "ZombiesClone/Public/Player/ZombieCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Zombie/Enemy/ZombieBase.h"

AWeapon1911::AWeapon1911()
{
    WeaponName = "1911";
    BaseDamage = 100;
    WeaponMaxAmmo = 98;
    MagazineMaxAmmo = 7;
    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;
}

TArray<FHitResult> AWeapon1911::Fire(AZombieCharacter* ShootingPlayer)
{
    UE_LOG(LogTemp, Warning, TEXT("Shooting 1911"));

    if (FireAnimation)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);
    
    if (HitResults.Num() > 0)
    {
        for (FHitResult& Result : HitResults)
        {
            if (AActor* HitActor = Result.GetActor())
            {
                if (AZombieBase* Zombie = Cast<AZombieBase>(HitActor))
                {
                    Zombie->Hit(ShootingPlayer);
                }

                UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
            }
        }
    }

    return HitResults;
}

void AWeapon1911::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("Reloading 1911"));
}
