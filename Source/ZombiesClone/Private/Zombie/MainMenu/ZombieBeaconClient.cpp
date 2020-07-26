// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconClient.h"

AZombieBeaconClient::AZombieBeaconClient()
{
    
}

bool AZombieBeaconClient::ConnectToServer(const FString& Address)
{
    FURL Destination = FURL(nullptr, *Address, ETravelType::TRAVEL_Absolute);
    Destination.Port = 7787;

    return InitClient(Destination);
}

void AZombieBeaconClient::OnConnected()
{
    UE_LOG(LogTemp, Warning, TEXT("BEACON CLIENT CONNECTED"));
    FConnected.Broadcast(true);
}

void AZombieBeaconClient::OnFailure()
{
    UE_LOG(LogTemp, Warning, TEXT("BEACON CLIENT FAILED TO CONNECT"));
    FConnected.Broadcast(false);
}
