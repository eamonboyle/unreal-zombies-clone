// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "ZombieBeaconHostObject.generated.h"

USTRUCT(BlueprintType)
struct FZombieLobbyInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    class UTexture2D* MapImage;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PlayerList;
};

UCLASS()
class ZOMBIESCLONE_API AZombieBeaconHostObject : public AOnlineBeaconHostObject
{
    GENERATED_BODY()

public:
    AZombieBeaconHostObject();

protected:
    FZombieLobbyInfo LobbyInfo;

    UFUNCTION(BlueprintCallable)
    void UpdateLobbyInfo(FZombieLobbyInfo NewLobbyInfo);

    void UpdateClientLobbyInfo();

protected:
    virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
    virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;

    UFUNCTION(BlueprintCallable)
    void ShutdownServer();

    void DisconnectAllClients();

    virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;
};
