// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Game/ZombieGameInstanceBase.h"
#include "Engine/World.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

UZombieGameInstanceBase::UZombieGameInstanceBase()
{
    // Http = &FHttpModule::Get();
}

TArray<FServerData>& UZombieGameInstanceBase::GetServerList()
{
    return ServerList;
}

void UZombieGameInstanceBase::GenerateServerList()
{
    // TSharedRef<IHttpRequest> Request = Http->CreateRequest();

    // Request->OnProcessRequestComplete().BindUObject(this, &UZombieGameInstanceBase::OnServerListRequestComplete);
    //
    // Request->SetURL("https://localhost:44386/api/Host");
    // Request->SetVerb("GET");
    // Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    //
    // Request->ProcessRequest();
}

// void UZombieGameInstanceBase::OnServerListRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response,
//                                                           bool Success)
// {
//     // retrieves the server data objects from the json response and then broadcasts
//     if (Success)
//     {
//         FString ResponseStr = Response->GetContentAsString();
//         ResponseStr.InsertAt(0, FString("{\"Response\":"));
//         ResponseStr.AppendChar('}');
//
//         TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
//         TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseStr);
//
//         if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
//         {
//             TArray<TSharedPtr<FJsonValue>> JsonValues = JsonObject->GetArrayField(TEXT("Response"));
//
//             ServerList.Empty();
//
//             for (TSharedPtr<FJsonValue> Value : JsonValues)
//             {
//                 FServerData ServerData = FServerData();
//                 TSharedPtr<FJsonObject> JsonObj = Value->AsObject();
//
//                 if (FJsonObjectConverter::JsonObjectToUStruct(JsonObj.ToSharedRef(), &ServerData, 0, 0))
//                 {
//                     ServerList.Add(ServerData);
//                 }
//             }
//
//             FOnServersReceived.Broadcast();
//         }
//     }
//     else
//     {
//         UE_LOG(LogTemp, Warning, TEXT("ServerListRequest FAILED"));
//     }
// }

void UZombieGameInstanceBase::GoToMap(FString MAPURL)
{
    GetWorld()->ServerTravel(MAPURL);
}
