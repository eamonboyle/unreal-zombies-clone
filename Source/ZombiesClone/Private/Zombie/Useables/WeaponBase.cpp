// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Useables/WeaponBase.h"
#include "ZombiesClone/Public/Player/ZombieCharacter.h"

#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    SetReplicates(true);

    WeaponName = "Default";
    BaseDamage = 100;
    WeaponRange = 5000.f;
    WeaponMaxAmmo = 255;
    MagazineMaxAmmo = 30;
    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
}

TArray<FHitResult> AWeaponBase::PerformLineTrace(AZombieCharacter* ShootingPlayer)
{
    // send out a ray trace in front of the character to see if it's shooting a zombie
    FVector Start = WeaponMesh->GetSocketLocation(FName("muzzleSocket"));
    FVector Rotation = WeaponMesh->GetSocketQuaternion(FName("muzzleSocket")).Vector();
    FVector End = Start + Rotation * WeaponRange;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    FCollisionResponseParams CollisionResponse;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(ShootingPlayer);
    
    GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel2,
                                                CollisionParams, CollisionResponse);

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

    return HitResults;
}

TArray<FHitResult> AWeaponBase::PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    FVector End = MuzzleLocation + MuzzleRotation.Vector() * WeaponRange;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    FCollisionResponseParams CollisionResponse;
    CollisionParams.AddIgnoredActor(this);

    if (GetOwner() != nullptr)
    {
        CollisionParams.AddIgnoredActor(GetOwner());
    }
    
    GetWorld()->LineTraceMultiByChannel(HitResults, MuzzleLocation, End, ECollisionChannel::ECC_GameTraceChannel2,
                                                CollisionParams, CollisionResponse);

    DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 2.0f, 0, 3.0f);

    return HitResults;
}

bool AWeaponBase::Server_Fire_Validate(const TArray<FHitResult>& HitResults)
{
    return true;
}

void AWeaponBase::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
}

TArray<FHitResult> AWeaponBase::Fire(AZombieCharacter* ShootingPlayer)
{
    return TArray<FHitResult>();
}

void AWeaponBase::Reload()
{
}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
    return {CurrentMagazineAmmo, CurrentTotalAmmo};
}

UAnimMontage* AWeaponBase::GetFireAnimMontage()
{
    return FPSArmsFireMontage;
}
