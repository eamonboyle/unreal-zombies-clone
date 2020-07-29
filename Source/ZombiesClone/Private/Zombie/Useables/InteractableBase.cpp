// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombiesClone/Public/Zombie/Useables/InteractableBase.h"

#include "ZombiesClone/Public/Player/CharacterBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
	ObjectName = "Default";
	UIMessage = TEXT("Press F to buy " + ObjectName);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FString AInteractableBase::GetUIMessage()
{
	return UIMessage;
}

void AInteractableBase::Use(AZombieCharacter* Player)
{
	
}

