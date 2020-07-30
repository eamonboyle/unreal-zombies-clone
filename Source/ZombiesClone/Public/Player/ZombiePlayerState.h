// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZombiePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsChanged, int32, NewPoints);

UCLASS()
class ZOMBIESCLONE_API AZombiePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AZombiePlayerState();

protected:
    UPROPERTY(BlueprintAssignable)
    FPointsChanged NewPoints;

    UPROPERTY(ReplicatedUsing = OnRep_PointsChanged, EditDefaultsOnly) // set to replicate
    int32 Points;
    UFUNCTION()
    void OnRep_PointsChanged();

public:
    void IncrementPoints(uint16 Value);
    bool DecrementPoints(uint16 Value);

    UFUNCTION(BlueprintCallable)
    int32 GetPoints();
};
