// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "ZombieBeaconClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FOnConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisconnected);

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
};
