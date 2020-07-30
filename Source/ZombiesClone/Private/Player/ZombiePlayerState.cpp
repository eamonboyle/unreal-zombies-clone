// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Player/ZombiePlayerState.h"

#include "Net/UnrealNetwork.h"

AZombiePlayerState::AZombiePlayerState()
{
    Points = 500;
}

void AZombiePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AZombiePlayerState, Points);
}

void AZombiePlayerState::OnRep_PointsChanged()
{
    NewPoints.Broadcast(Points);
}


void AZombiePlayerState::IncrementPoints(uint16 Value)
{
    Points += Value;

    if (HasAuthority())
    {
        OnRep_PointsChanged();
    }

    NewPoints.Broadcast(Points);
    UE_LOG(LogTemp, Warning, TEXT("Points: %d"), Points);
}

bool AZombiePlayerState::DecrementPoints(uint16 Value)
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

int32 AZombiePlayerState::GetPoints()
{
    return Points;
}
