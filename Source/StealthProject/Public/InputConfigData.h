// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* IA_Move;

};
