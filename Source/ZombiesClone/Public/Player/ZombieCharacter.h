// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CharacterBase.h"
#include "ZombieCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractChanged, const FString&, OnInteractChanged);

UCLASS()
class ZOMBIESCLONE_API AZombieCharacter : public ACharacterBase
{
    GENERATED_BODY()

public:
    AZombieCharacter();

protected:
    UPROPERTY(BlueprintAssignable)
    FInteractChanged OnInteractChanged;
    
    FTimerHandle TInteractTimerHandle;
    class AInteractableBase* Interactable;

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
    void Interact();
};
