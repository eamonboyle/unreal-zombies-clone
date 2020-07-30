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

void AWeapon1911::Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    UE_LOG(LogTemp, Warning, TEXT("SERVER FIRE FUNCTION"));

    // play the fire animation on the gun
    if (FireAnimation)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    // get the hit results from the line tracing in the base class
    TArray<FHitResult> HitResults = PerformLineTrace(MuzzleLocation, MuzzleRotation);

    if (HitResults.Num() > 0)
    {
        for (FHitResult& Result : HitResults)
        {
            if (AActor* HitActor = Result.GetActor())
            {
                // if we have hit a zombie, run the Hit function in the Zombie Class
                if (AZombieBase* Zombie = Cast<AZombieBase>(HitActor))
                {
                    if (AZombieCharacter* Player = Cast<AZombieCharacter>(GetOwner()))
                    {
                        Zombie->Hit(Player);
                    }
                }

                UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
            }
        }
    }
}

TArray<FHitResult> AWeapon1911::Fire(AZombieCharacter* ShootingPlayer)
{
    UE_LOG(LogTemp, Warning, TEXT("Shooting 1911"));

    // play the fire animation on the gun
    if (FireAnimation)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    if (GetWorld()->IsServer())
    {
        // get the hit results from the line tracing in the base class
        TArray<FHitResult> HitResults = PerformLineTrace(ShootingPlayer);

        if (HitResults.Num() > 0)
        {
            for (FHitResult& Result : HitResults)
            {
                if (AActor* HitActor = Result.GetActor())
                {
                    // if we have hit a zombie, run the Hit function in the Zombie Class
                    if (AZombieBase* Zombie = Cast<AZombieBase>(HitActor))
                    {
                        Zombie->Hit(ShootingPlayer);
                    }

                    UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
                }
            }
        }
    }
    else
    {
        Server_Fire(WeaponMesh->GetSocketLocation(FName("muzzleSocket")),
                    WeaponMesh->GetSocketRotation(FName("muzzleSocket")));
    }

    return TArray<FHitResult>();
}

void AWeapon1911::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("Reloading 1911"));
}
