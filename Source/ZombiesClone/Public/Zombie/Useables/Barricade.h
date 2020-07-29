// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Zombie/Useables/InteractableBase.h"
#include "Barricade.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESCLONE_API ABarricade : public AInteractableBase
{
    GENERATED_BODY()

public:
    ABarricade();

protected:
    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    class UStaticMeshComponent* MeshComp;

    UPROPERTY(EditAnywhere, Category = "Zombie Settings")
    uint16 Cost;

    UPROPERTY(ReplicatedUsing = OnRep_BarricadeUsed)
    bool bIsUsed;

    UFUNCTION()
    void OnRep_BarricadeUsed();

protected:
    virtual void BeginPlay() override;
    virtual void Use(ACharacterBase* Player) override;
};
