// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Player/ZombieCharacter.h"

#include "DrawDebugHelpers.h"
#include "ZombiesClone/Public/Zombie/Useables/InteractableBase.h"
#include "ZombiesClone/Public/Zombie/Enemy/ZombieBase.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"

AZombieCharacter::AZombieCharacter()
{
    Interactable = nullptr;
    InteractionRange = 200.f;
    Points = 500;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
    Super::BeginPlay();

    // setup the interact timer
    GetWorld()->GetTimerManager().SetTimer(TInteractTimerHandle, this, &AZombieCharacter::SetInteractableObject, 0.2f,
                                           true);
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // set up gameplay key bindings
    check(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AZombieCharacter::Interact);
}

void AZombieCharacter::OnFire()
{
    UE_LOG(LogTemp, Warning, TEXT("Shooting Weapon"));
    
    // send out a ray trace in front of the character to see if it's shooting a zombie
    FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
    FVector Rotation = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
    FVector End = Start + Rotation * 10000.0f;

    FHitResult HitResult;
    FCollisionObjectQueryParams CollisionQuery;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    
    if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, CollisionQuery, CollisionParams))
    {
        if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
        {
            UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT %s"), *Zombie->GetName());
            Zombie->Hit(this);
        }
    }

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);
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

void AZombieCharacter::IncrementPoints(uint16 Value)
{
    Points += Value;
    UE_LOG(LogTemp, Warning, TEXT("Points: %d"), Points);
}

bool AZombieCharacter::DecrementPoints(uint16 Value)
{
    if ((Points - Value) < 0)
    {
        return false;
    }
    else
    {
        Points -= Value;
    }

    UE_LOG(LogTemp, Warning, TEXT("Points: %d"), Points);

    return true;
}
