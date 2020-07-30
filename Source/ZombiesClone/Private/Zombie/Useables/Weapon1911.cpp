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

    // send out a ray trace in front of the character to see if it's shooting a zombie
    FVector Start = WeaponMesh->GetSocketLocation(FName("muzzleSocket"));
    FVector Rotation = WeaponMesh->GetSocketQuaternion(FName("muzzleSocket")).Vector();
    FVector End = Start + Rotation * 10000.0f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    FCollisionResponseParams CollisionResponse;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(ShootingPlayer);

    if (GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel2,
                                            CollisionParams, CollisionResponse))
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

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

    return HitResults;
}

void AWeapon1911::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("Reloading 1911"));
}
