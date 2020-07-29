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
    class UAnimationAsset* ReloadAnimation;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    FString WeaponName;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    int32 WeaponMaxAmmo;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    int32 MagazineMaxAmmo;

    int32 CurrentTotalAmmo;
    int32 CurrentMagazineAmmo;


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    virtual void Fire();
    virtual void Reload();

    /** first element is magazine ammo, second element is total ammo */
    TArray<int32> GetCurrentAmmo();
};
