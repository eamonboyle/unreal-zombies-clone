// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "Http.h"

#include "ZombieBeaconHostObject.generated.h"

USTRUCT(BlueprintType)
struct FZombieLobbyInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    class UTexture2D* MapImage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString MapName;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PlayerList;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHostLobbyUpdated, FZombieLobbyInfo, FOnHostLobbyUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHostChatReceived, const FText&, FOnHostChatReceived);

UCLASS()
class ZOMBIESCLONE_API AZombieBeaconHostObject : public AOnlineBeaconHostObject
{
    GENERATED_BODY()

public:
    AZombieBeaconHostObject();

protected:
    FHttpModule* Http;
    int ServerID;

    void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

protected:
    FZombieLobbyInfo LobbyInfo;

    UFUNCTION(BlueprintCallable)
    void SetServerData(const FString& ServerName, const FString& MapName, int CurrentPlayers, int MaxPlayers);

    UFUNCTION(BlueprintCallable)
    int GetCurrentPlayerCount();

    UPROPERTY(BlueprintAssignable)
    FHostLobbyUpdated FOnHostLobbyUpdated;

    UPROPERTY(BlueprintAssignable)
    FHostChatReceived FOnHostChatReceived;

    FTimerHandle TInitialLobbyHandle;
    void InitialLobbyHandling();

    UFUNCTION(BlueprintCallable)
    void UpdateLobbyInfo(FZombieLobbyInfo NewLobbyInfo);

    void UpdateClientLobbyInfo();

protected:
    virtual void BeginPlay() override;

    virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
    virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;

    UFUNCTION(BlueprintCallable)
    void ShutdownServer();

    void DisconnectAllClients();

    virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;

public:
    UFUNCTION(BlueprintCallable)
    void SendChatToLobby(const FText& ChatMessage);
};
