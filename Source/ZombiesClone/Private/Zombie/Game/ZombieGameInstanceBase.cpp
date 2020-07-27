// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameInstanceBase.h"
#include "Engine/World.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

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
        FString ResponseStr = Response->GetContentAsString();
        ResponseStr.InsertAt(0, FString("{\"Response\":"));
        ResponseStr.AppendChar('}');

        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseStr);

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
        {
            TArray<TSharedPtr<FJsonValue>> JsonValues = JsonObject->GetArrayField(TEXT("Response"));
            TArray<FServerData> ServerList;

            for (TSharedPtr<FJsonValue> Value : JsonValues)
            {
                FServerData ServerData = FServerData();
                TSharedPtr<FJsonObject> JsonObj = Value->AsObject();

                if (FJsonObjectConverter::JsonObjectToUStruct(JsonObj.ToSharedRef(), &ServerData, 0, 0))
                {
                    ServerList.Add(ServerData);
                }
            }

            for (FServerData ServerData : ServerList)
            {
                UE_LOG(LogTemp, Warning, TEXT("ServerID: %d"), ServerData.ServerID);
                UE_LOG(LogTemp, Warning, TEXT("IP: %s"), *ServerData.IPAddress);
                UE_LOG(LogTemp, Warning, TEXT("ServerName: %s"), *ServerData.ServerName);
                UE_LOG(LogTemp, Warning, TEXT("MapName: %s"), *ServerData.MapName);
                UE_LOG(LogTemp, Warning, TEXT("CurrentPlayers: %d"), ServerData.CurrentPlayers);
                UE_LOG(LogTemp, Warning, TEXT("MaxPlayers: %d"), ServerData.MaxPlayers);
                UE_LOG(LogTemp, Warning, TEXT("----------------------------------------"));
            }
        }
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
