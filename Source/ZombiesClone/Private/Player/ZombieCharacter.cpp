// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Player/ZombieCharacter.h"

AZombieCharacter::AZombieCharacter()
{
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
    Super::BeginPlay();

    //Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
    FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
                              TEXT("GripPoint"));
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // set up gameplay key bindings
    check(PlayerInputComponent);
}