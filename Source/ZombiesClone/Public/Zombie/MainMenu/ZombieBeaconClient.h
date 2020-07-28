// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconHostObject.h"
#include "ZombieBeaconClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FOnConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyUpdated, FZombieLobbyInfo, FOnLobbyUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChatReceived, const FText&, FOnChatReceived);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFullConnect);

UCLASS()
class ZOMBIESCLONE_API AZombieBeaconClient : public AOnlineBeaconClient
{
    GENERATED_BODY()

public:
    AZombieBeaconClient();

protected:
    UPROPERTY(BlueprintAssignable)
    FConnectSuccess FOnConnected;

    UPROPERTY(BlueprintAssignable)
    FDisconnected FOnDisconnected;

    UPROPERTY(BlueprintAssignable)
    FLobbyUpdated FOnLobbyUpdated;

    UPROPERTY(BlueprintAssignable)
    FChatReceived FOnChatReceived;

    UPROPERTY(BlueprintAssignable)
    FFullConnect FOnFullConnect;

    uint8 PlayerIndex;
    FString PlayerName;

protected:
    UFUNCTION(BlueprintCallable)
    bool ConnectToServer(const FString& Address);

    UFUNCTION(BlueprintCallable)
    void LeaveLobby();

    virtual void OnConnected() override;
    virtual void OnFailure() override;

protected:
    UFUNCTION(BlueprintCallable)
    void SendChatMessage(const FText& ChatMessage);
    
    UFUNCTION(BlueprintCallable)
    void FullConnectToServer(const FString& JoinAddress);

    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SendChatMessage(const FText& ChatMessage);
    bool Server_SendChatMessage_Validate(const FText& ChatMessage);
    void Server_SendChatMessage_Implementation(const FText& ChatMessage);

public:
    UFUNCTION(Client, Reliable)
    virtual void Client_OnDisconnected();
    virtual void Client_OnDisconnected_Implementation();

    UFUNCTION(Client, Reliable)
    void Client_OnLobbyUpdated(FZombieLobbyInfo LobbyInfo);
    void Client_OnLobbyUpdated_Implementation(FZombieLobbyInfo LobbyInfo);
    
    UFUNCTION(Client, Reliable)
    virtual void Client_OnChatMessageReceived(const FText& ChatMessage);
    virtual void Client_OnChatMessageReceived_Implementation(const FText& ChatMessage);
    
    UFUNCTION(Client, Reliable)
    virtual void Client_FullConnect();
    virtual void Client_FullConnect_Implementation();

    void SetPlayerIndex(uint8 Index);
    uint8 GetPlayerIndex();

    void SetPlayerName(const FString& NewPlayerName);
    FString GetPlayerName();
};
