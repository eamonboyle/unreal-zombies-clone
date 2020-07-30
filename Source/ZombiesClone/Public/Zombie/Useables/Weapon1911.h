// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombie/Useables/WeaponBase.h"
#include "Weapon1911.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESCLONE_API AWeapon1911 : public AWeaponBase
{
    GENERATED_BODY()

public:
    AWeapon1911();

protected:
    virtual void Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

public:
    virtual TArray<FHitResult> Fire(class AZombieCharacter* ShootingPlayer) override;
    virtual void Reload() override;
};
