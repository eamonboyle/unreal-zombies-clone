// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Player/ZombieCharacter.h"
#include "ZombiesClone/Public/Zombie/Useables/Weapon1911.h"
#include "ZombiesClone/Public/Zombie/Useables/InteractableBase.h"
#include "ZombiesClone/Public/Zombie/Enemy/ZombieBase.h"

#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"

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

void AZombieCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AZombieCharacter, Points);
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
    if (CurrentWeapon != nullptr)
    {
        CurrentWeapon->Fire(this);
        
        if (UAnimMontage* FireMontage = CurrentWeapon->GetFireAnimMontage())
        {
            UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();

            if (AnimInstance != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Got the Anim Instance on the M1911"));
                AnimInstance->Montage_Play(FireMontage, 1.f);
            }
        }
    }
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
        NewInteractMessage.Broadcast(Interactable->GetUIMessage());
    }
    else if (Interactable && Temp == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("IS NOW A NULL PTR"));
        Interactable = nullptr;
        NewInteractMessage.Broadcast(FString());
    }
}

void AZombieCharacter::OnRep_PointsChanged()
{
    NewPoints.Broadcast(Points);
}

void AZombieCharacter::IncrementPoints(uint16 Value)
{
    if (HasAuthority())
    {
        Points += Value;
        OnRep_PointsChanged();
        UE_LOG(LogTemp, Warning, TEXT("Points: %d"), Points);
    }
}

bool AZombieCharacter::DecrementPoints(uint16 Value)
{
    if (HasAuthority())
    {
        if ((Points - Value) < 0)
        {
            return false;
        }
        else
        {
            Points -= Value;
        }

        OnRep_PointsChanged();
        UE_LOG(LogTemp, Warning, TEXT("Points: %d"), Points);
    }

    return true;
}

int32 AZombieCharacter::GetPoints()
{
    return Points;
}
