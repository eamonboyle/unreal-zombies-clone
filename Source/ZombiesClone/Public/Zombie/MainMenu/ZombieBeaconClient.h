// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "ZombieBeaconClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSuccess, bool, FConnected);

UCLASS()
class ZOMBIESCLONE_API AZombieBeaconClient : public AOnlineBeaconClient
{
    GENERATED_BODY()

public:
    AZombieBeaconClient();

protected:
    UPROPERTY(BlueprintAssignable)
    FConnectSuccess FConnected;

protected:
    UFUNCTION(BlueprintCallable)
    bool ConnectToServer(const FString& Address);

    virtual void OnConnected() override;
    virtual void OnFailure() override;
};
