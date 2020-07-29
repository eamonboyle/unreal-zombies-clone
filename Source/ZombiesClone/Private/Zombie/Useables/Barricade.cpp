// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Useables/Barricade.h"

#include "Components/StaticMeshComponent.h"

ABarricade::ABarricade()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

    RootComponent = MeshComp;
    Cost = 500;
    UIMessage += FString(" [Cost: " + FString::FromInt(Cost) + "]");
}

void ABarricade::BeginPlay()
{
    Super::BeginPlay();
}