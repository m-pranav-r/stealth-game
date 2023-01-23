// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthPlayerController.h"

void AStealthPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

APlayerCharacter* AStealthPlayerController::GetPlayer()
{
    return PlayerReference;
}

void AStealthPlayerController::SetPlayerReference(APlayerCharacter* PlayerPointer)
{
    PlayerReference = PlayerPointer;
}
