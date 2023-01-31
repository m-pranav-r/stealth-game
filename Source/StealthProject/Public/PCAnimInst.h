// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PCAnimInst.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UPCAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool isJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool isMoving = false;
};
