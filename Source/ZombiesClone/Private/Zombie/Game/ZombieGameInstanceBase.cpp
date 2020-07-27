// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameInstanceBase.h"
#include "Engine/World.h"

UZombieGameInstanceBase::UZombieGameInstanceBase()
{
    Http = &FHttpModule::Get();
}

void UZombieGameInstanceBase::GetServerList()
{
    TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(this, &UZombieGameInstanceBase::OnServerListRequestComplete);

    Request->SetURL("https://waw-master-server.azurewebsites.net/api/Host");
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    Request->ProcessRequest();
}

void UZombieGameInstanceBase::OnServerListRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                          bool Success)
{
    if (Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("ServerListRequest SUCCESS: %s"), *Response->GetContentAsString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ServerListRequest FAILED"));
    }
}

void UZombieGameInstanceBase::GoToMap(FString MAPURL)
{
    GetWorld()->ServerTravel(MAPURL);
}
