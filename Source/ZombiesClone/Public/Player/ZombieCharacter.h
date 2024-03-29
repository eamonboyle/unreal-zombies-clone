// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "ZombieCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, NewInteractMessage);

UCLASS()
class ZOMBIESCLONE_API AZombieCharacter : public ACharacterBase
{
    GENERATED_BODY()

public:
    AZombieCharacter();

protected:
    UPROPERTY(BlueprintAssignable)
    FInteractChanged NewInteractMessage;

    FTimerHandle TInteractTimerHandle;
    class AInteractableBase* Interactable;

    UPROPERTY(EditDefaultsOnly)
    float InteractionRange;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void OnFire() override;

protected:
    void Interact();
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_Interact(class AInteractableBase* InteractingObject);
    bool Server_Interact_Validate(class AInteractableBase* InteractingObject);
    void Server_Interact_Implementation(class AInteractableBase* InteractingObject);
    
    void SetInteractableObject();
};
