// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Player/ZombieCharacter.h"
#include "ZombiesClone/Public/Zombie/Useables/InteractableBase.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"

AZombieCharacter::AZombieCharacter()
{
    Interactable = nullptr;
    InteractionRange = 200.f;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
    Super::BeginPlay();

    //Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
    FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
                              TEXT("GripPoint"));

    // setup the interact timer
    GetWorld()->GetTimerManager().SetTimer(TInteractTimerHandle, this, &AZombieCharacter::SetInteractableObject, 0.2f, true);
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // set up gameplay key bindings
    check(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AZombieCharacter::Interact);
}

void AZombieCharacter::Interact()
{
    UE_LOG(LogTemp, Warning, TEXT("INTERACTING"));

    // perform server RPC if client
    if (Interactable)
    {
        if (HasAuthority())
        {
            Interactable->Use(this);
        }
        else
        {
            Server_Interact(Interactable);
        }
    }
}

bool AZombieCharacter::Server_Interact_Validate(AInteractableBase* InteractingObject)
{
    return true;
}

void AZombieCharacter::Server_Interact_Implementation(AInteractableBase* InteractingObject)
{
    float Distance = GetDistanceTo(InteractingObject);
    UE_LOG(LogTemp, Warning, TEXT("DISTANCE: %f - %f"), Distance, InteractionRange + 30.f);

    if (Distance < InteractionRange + 30.f)
    {
        InteractingObject->Use(this);
    }
}

void AZombieCharacter::SetInteractableObject()
{
    // send out a ray trace in front of the character to see if it's looking at an interactable
    FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
    FVector Rotation = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
    FVector End = Start + Rotation * InteractionRange;

    FHitResult HitResult;
    FCollisionObjectQueryParams CollisionQuery;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, CollisionQuery, CollisionParams);
    
    AInteractableBase* Temp = Cast<AInteractableBase>(HitResult.GetActor());
    if (Interactable == nullptr && Temp)
    {
        UE_LOG(LogTemp, Warning, TEXT("IS NOW A VALID PTR"));
        Interactable = Temp;
        OnInteractChanged.Broadcast(Interactable->GetUIMessage());
    }
    else if (Interactable && Temp == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("IS NOW A NULL PTR"));
        Interactable = nullptr;
        OnInteractChanged.Broadcast(FString());
    }
}
