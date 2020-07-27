// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Http.h"

#include "ZombieGameInstanceBase.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString MapURL;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString MapName;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString MapDescription;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    class UTexture2D* MapImage;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServersReceived);

UCLASS()
class ZOMBIESCLONE_API UZombieGameInstanceBase : public UGameInstance
{
    GENERATED_BODY()

public:
    UZombieGameInstanceBase();

protected:
    FHttpModule* Http;

    UPROPERTY(BlueprintAssignable)
    FServersReceived FOnServersReceived;

    UFUNCTION(BlueprintCallable)
    void GetServerList();

    void OnServerListRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);

protected:
    UFUNCTION(BlueprintCallable)
    void GoToMap(FString MAPURL);
};
