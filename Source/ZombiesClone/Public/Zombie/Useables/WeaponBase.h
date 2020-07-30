// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class ZOMBIESCLONE_API AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AWeaponBase();

protected:
    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    class UAnimationAsset* FireAnimation;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    class UAnimMontage* FPSArmsFireMontage;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    class UAnimationAsset* ReloadAnimation;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    FString WeaponName;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    int32 BaseDamage;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    float WeaponRange;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    int32 WeaponMaxAmmo;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    int32 MagazineMaxAmmo;

    int32 CurrentTotalAmmo;
    int32 CurrentMagazineAmmo;


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    TArray<FHitResult> PerformLineTrace(class AZombieCharacter* ShootingPlayer);
    TArray<FHitResult> PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
    bool Server_Fire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);
    virtual void Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

public:
    virtual TArray<FHitResult> Fire(class AZombieCharacter* ShootingPlayer);
    virtual void Reload();

    /** first element is magazine ammo, second element is total ammo */
    TArray<int32> GetCurrentAmmo();

    class UAnimMontage* GetFireAnimMontage();
};
