// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Useables/Barricade.h"

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
}

void ABarricade::BeginPlay()
{
    Super::BeginPlay();
    SetReplicates(true);
}

void ABarricade::Use(ACharacterBase* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("USING %s"), *ObjectName);

    // play animation to move barricade
    bIsUsed = true;
    OnRep_BarricadeUsed();
}
