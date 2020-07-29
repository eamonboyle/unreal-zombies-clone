// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Useables/Weapon1911.h"

AWeapon1911::AWeapon1911()
{
    WeaponName = "1911";
    WeaponMaxAmmo = 98;
    MagazineMaxAmmo = 7;
    CurrentTotalAmmo = WeaponMaxAmmo;
    CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeapon1911::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Shooting 1911"));
}

void AWeapon1911::Reload()
{
    UE_LOG(LogTemp, Warning, TEXT("Reloading 1911"));
}