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

void AZombieBeaconClient::SendChatMessage(const FText& ChatMessage)
{
    Server_SendChatMessage(ChatMessage);
}

bool AZombieBeaconClient::Server_SendChatMessage_Validate(const FText& ChatMessage)
{
    return true;
}

void AZombieBeaconClient::Server_SendChatMessage_Implementation(const FText& ChatMessage)
{
    UE_LOG(LogTemp, Warning ,TEXT("Chat: %s"), *ChatMessage.ToString());

    if (AZombieBeaconHostObject* Host = Cast<AZombieBeaconHostObject>(BeaconOwner))
    {
        Host->SendChatToLobby(ChatMessage);
    }
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

void AZombieBeaconClient::Client_OnChatMessageReceived_Implementation(const FText& ChatMessage)
{
    FOnChatReceived.Broadcast(ChatMessage);
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
