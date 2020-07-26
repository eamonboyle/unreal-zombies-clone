// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconHostObject.h"
#include "ZombieBeaconClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FOnConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyUpdated, FZombieLobbyInfo, FOnLobbyUpdated);

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

protected:
    UFUNCTION(BlueprintCallable)
    bool ConnectToServer(const FString& Address);

    UFUNCTION(BlueprintCallable)
    void LeaveLobby();

    virtual void OnConnected() override;
    virtual void OnFailure() override;

public:
    UFUNCTION(Client, Reliable)
    virtual void Client_OnDisconnected();
    virtual void Client_OnDisconnected_Implementation();

    UFUNCTION(Client, Reliable)
    void Client_OnLobbyUpdated(FZombieLobbyInfo LobbyInfo);
    void Client_OnLobbyUpdated_Implementation(FZombieLobbyInfo LobbyInfo);
};
