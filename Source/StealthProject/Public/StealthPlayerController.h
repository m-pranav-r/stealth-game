// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StealthPlayerController.generated.h"

UCLASS()
class STEALTHPROJECT_API AStealthPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	protected:

	virtual void BeginPlay() override;

protected:
	
	class APlayerCharacter* PlayerReference = nullptr;

public:

	APlayerCharacter* GetPlayer();

	void SetPlayerReference(APlayerCharacter* PlayerPointer);
};
