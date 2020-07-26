// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconHostObject.h"
#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconClient.h"
#include "ZombiesClone/Public/Zombie/MainMenu/ZombieMainMenuGameMode.h"

#include "OnlineBeaconHost.h"

AZombieBeaconHostObject::AZombieBeaconHostObject()
{
    ClientBeaconActorClass = AZombieBeaconClient::StaticClass();
    BeaconTypeName = ClientBeaconActorClass->GetName();
}

void AZombieBeaconHostObject::UpdateLobbyInfo(FZombieLobbyInfo NewLobbyInfo)
{
    LobbyInfo.MapImage = NewLobbyInfo.MapImage;
    LobbyInfo.MapName = NewLobbyInfo.MapName;
    UpdateClientLobbyInfo();
    FOnHostLobbyUpdated.Broadcast(LobbyInfo);
}

void AZombieBeaconHostObject::UpdateClientLobbyInfo()
{
    for (AOnlineBeaconClient* ClientBeacon : ClientActors)
    {
        if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
        {
            Client->Client_OnLobbyUpdated(LobbyInfo);
        }
    }
}

void AZombieBeaconHostObject::BeginPlay()
{
    Super::BeginPlay();

    // set the host as the first player in the player list
    LobbyInfo.PlayerList.Add(FString("Host Player"));
}

void AZombieBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
    Super::OnClientConnected(NewClientActor, ClientConnection);

    if (NewClientActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("CONNECTED CLIENT VALID"));

        FString PlayerName = FString("Player ");
        PlayerName.Append(FString::FromInt(LobbyInfo.PlayerList.Num()));
        LobbyInfo.PlayerList.Add(PlayerName);

        FOnHostLobbyUpdated.Broadcast(LobbyInfo);
        
        UpdateClientLobbyInfo();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CONNECTED CLIENT VALID"));
    }
}

void AZombieBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
    Super::NotifyClientDisconnected(LeavingClientActor);

    UE_LOG(LogTemp, Warning, TEXT("Client has disconnected"));
}

void AZombieBeaconHostObject::ShutdownServer()
{
    // unregister server from database via Web API
    DisconnectAllClients();

    // when all clients are disconnected get the beacon host and unregister it 
    if (AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Destroying host beacon"));
        Host->UnregisterHost(BeaconTypeName);
        Host->DestroyBeacon();
    }
}

void AZombieBeaconHostObject::DisconnectAllClients()
{
    UE_LOG(LogTemp, Warning, TEXT("Disconnected all clients"));

    for (AOnlineBeaconClient* Client : ClientActors)
    {
        if (Client != nullptr)
        {
            DisconnectClient(Client);
        }
    }
}

void AZombieBeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
{
    AOnlineBeaconHost* BeaconHost = Cast<AOnlineBeaconHost>(GetOwner());

    if (BeaconHost)
    {
        if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientActor))
        {
            UE_LOG(LogTemp, Warning, TEXT("DISCONNECTING CLIENT %s"), *ClientActor->GetName());
            Client->Client_OnDisconnected();
        }

        BeaconHost->DisconnectClient(ClientActor);
    }
}
