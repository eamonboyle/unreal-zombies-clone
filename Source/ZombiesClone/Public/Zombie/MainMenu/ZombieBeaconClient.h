// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "ZombieBeaconClient.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESCLONE_API AZombieBeaconClient : public AOnlineBeaconClient
{
    GENERATED_BODY()

public:
    AZombieBeaconClient();

protected:
    UFUNCTION(BlueprintCallable)
    bool ConnectToServer(const FString& Address);

    virtual void OnConnected() override;
    virtual void OnFailure() override;
};
