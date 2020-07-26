// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconClient.h"

AZombieBeaconClient::AZombieBeaconClient()
{
    PlayerIndex = 0;
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
    FOnConnected.Broadcast(true);
}

void AZombieBeaconClient::OnFailure()
{
    UE_LOG(LogTemp, Warning, TEXT("BEACON CLIENT FAILED TO CONNECT"));
    FOnConnected.Broadcast(false);
}

void AZombieBeaconClient::Client_OnDisconnected_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("DISCONNECTED_Implementation"));
    FOnDisconnected.Broadcast();
}

void AZombieBeaconClient::Client_OnLobbyUpdated_Implementation(FZombieLobbyInfo LobbyInfo)
{
    FOnLobbyUpdated.Broadcast(LobbyInfo);
}

void AZombieBeaconClient::SetPlayerIndex(uint8 Index)
{
    PlayerIndex = Index;
}

uint8 AZombieBeaconClient::GetPlayerIndex()
{
    return PlayerIndex;
}

void AZombieBeaconClient::LeaveLobby()
{
    DestroyBeacon();
}
