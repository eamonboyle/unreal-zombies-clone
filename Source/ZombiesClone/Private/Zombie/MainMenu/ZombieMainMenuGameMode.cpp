// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/MainMenu/ZombieMainMenuGameMode.h"
#include "ZombiesClone/Public/Zombie/MainMenu/ZombieBeaconHostObject.h"

#include "OnlineBeaconHost.h"

AZombieMainMenuGameMode::AZombieMainMenuGameMode()
{
    HostObject = nullptr;
}

bool AZombieMainMenuGameMode::CreateHostBeacon()
{
    // tries to create a host beacon, if it can register our HostObject it will return true
    // otherwise it will return false
    if (AOnlineBeaconHost* Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass()))
    {
        if (Host->InitHost())
        {
            Host->PauseBeaconRequests(false);

            HostObject = GetWorld()->SpawnActor<AZombieBeaconHostObject>(AZombieBeaconHostObject::StaticClass());

            if (HostObject != nullptr)
            {
                Host->RegisterHost(HostObject);
            
                return true;
            }
        }
    }

    return false;
}
