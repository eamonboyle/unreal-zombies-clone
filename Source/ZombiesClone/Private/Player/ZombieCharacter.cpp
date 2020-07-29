// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Player/ZombieCharacter.h"
#include "ZombiesClone/Public/Zombie/Useables/InteractableBase.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"

AZombieCharacter::AZombieCharacter()
{
    Interactable = nullptr;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
    Super::BeginPlay();

    //Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
    FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
                              TEXT("GripPoint"));

    // setup the interact timer
    GetWorld()->GetTimerManager().SetTimer(TInteractTimerHandle, this, &AZombieCharacter::Interact, 0.2f, true);
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // set up gameplay key bindings
    check(PlayerInputComponent);
}

void AZombieCharacter::Interact()
{
    // send out a ray trace infront of the character to see if it's looking at an interactable
    FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
    FVector Rotation = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
    FVector End = Start + Rotation * 500.f;

    FHitResult HitResult;
    FCollisionObjectQueryParams CollisionQuery;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, CollisionQuery, CollisionParams);

    if (AInteractableBase* Temp = Cast<AInteractableBase>(HitResult.GetActor()))
    {
        Interactable = Temp;
        UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *HitResult.GetActor()->GetName());
    }
    else
    {
        Interactable = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("Did not hit anything important"));
    }
}
