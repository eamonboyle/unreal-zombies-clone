// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconHostObject.h"
#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconClient.h"
#include "ZombiesClone/Public/Zombie/MainMenu/ZombieMainMenuGameMode.h"

#include "OnlineBeaconHost.h"
#include "TimerManager.h"

AZombieBeaconHostObject::AZombieBeaconHostObject()
{
    ClientBeaconActorClass = AZombieBeaconClient::StaticClass();
    BeaconTypeName = ClientBeaconActorClass->GetName();

    Http = &FHttpModule::Get();
    ServerID = -1;
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

    FOnHostLobbyUpdated.Broadcast(LobbyInfo);
}

void AZombieBeaconHostObject::BeginPlay()
{
    Super::BeginPlay();

    // set the host as the first player in the player list
    LobbyInfo.PlayerList.Add(FString("Host Player"));

    GetWorld()->GetTimerManager().SetTimer(TInitialLobbyHandle, this, &AZombieBeaconHostObject::InitialLobbyHandling,
                                           0.2f, false);
}

void AZombieBeaconHostObject::InitialLobbyHandling()
{
    UpdateLobbyInfo(LobbyInfo);
}

void AZombieBeaconHostObject::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
    if (Success)
    {
        ServerID = FCString::Atoi(*Response->GetContentAsString());
        UE_LOG(LogTemp, Warning, TEXT("Success, ID: %d"), ServerID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HttpRequest FAILED"));
    }
}

void AZombieBeaconHostObject::SetServerData(FServerData NewServerData)
{
    ServerData = NewServerData;
    ServerData.CurrentPlayers = GetCurrentPlayerCount();

    // construct json object to send to the server
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetNumberField("ServerID", 0);
    JsonObject->SetStringField("IPAddress", "26153176253");
    JsonObject->SetStringField("ServerName", ServerData.ServerName);
    JsonObject->SetStringField("MapName", ServerData.MapName);
    JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
    JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);

    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &AZombieBeaconHostObject::OnProcessRequestComplete);

    Request->SetURL("https://localhost:44386/api/Host");
    Request->SetVerb("POST");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    Request->SetContentAsString(JsonString);

    Request->ProcessRequest();
}

void AZombieBeaconHostObject::UpdateServerData(FServerData NewServerData)
{
    ServerData = NewServerData;
    ServerData.CurrentPlayers = GetCurrentPlayerCount();

    // construct json object to send to the server
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetNumberField("ServerID", 0);
    JsonObject->SetStringField("IPAddress", "26153176253");
    JsonObject->SetStringField("ServerName", ServerData.ServerName);
    JsonObject->SetStringField("MapName", ServerData.MapName);
    JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
    JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);

    FString JsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);

    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &AZombieBeaconHostObject::OnProcessRequestComplete);

    Request->SetURL("https://localhost:44386/api/Host");
    Request->SetVerb("PUT");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    Request->SetContentAsString(JsonString);

    Request->ProcessRequest();
}

int AZombieBeaconHostObject::GetCurrentPlayerCount()
{
    return LobbyInfo.PlayerList.Num();
}


void AZombieBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
    Super::OnClientConnected(NewClientActor, ClientConnection);

    if (NewClientActor)
    {
        FString PlayerName = FString("Player ");
        uint8 Index = LobbyInfo.PlayerList.Num();
        PlayerName.Append(FString::FromInt(Index));
        LobbyInfo.PlayerList.Add(PlayerName);

        if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(NewClientActor))
        {
            Client->SetPlayerIndex(Index);
            Client->SetPlayerName(PlayerName);
        }

        UE_LOG(LogTemp, Warning, TEXT("CONNECTED CLIENT VALID"));
        UpdateClientLobbyInfo();

        UpdateServerData(ServerData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CONNECTED CLIENT INVALID"));
    }
}

void AZombieBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
    Super::NotifyClientDisconnected(LeavingClientActor);

    UE_LOG(LogTemp, Warning, TEXT("Client has disconnected"));

    if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(LeavingClientActor))
    {
        uint8 Index = Client->GetPlayerIndex();
        LobbyInfo.PlayerList.RemoveAt(Index);
    }

    UpdateClientLobbyInfo();
    UpdateServerData(ServerData);
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

    if (ServerID != -1)
    {
        // delete the server entry on the Master Server
        TSharedRef<IHttpRequest> Request = Http->CreateRequest();

        Request->SetURL("https://localhost:44386/api/Host/" + FString::FromInt(ServerID));
        Request->SetVerb("DELETE");
        Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

        Request->ProcessRequest();
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

void AZombieBeaconHostObject::SendChatToLobby(const FText& ChatMessage)
{
    FOnHostChatReceived.Broadcast(ChatMessage);

    for (AOnlineBeaconClient* ClientBeacon : ClientActors)
    {
        if (AZombieBeaconClient* Client = Cast<AZombieBeaconClient>(ClientBeacon))
        {
            Client->Client_OnChatMessageReceived(ChatMessage);
        }
    }
}
