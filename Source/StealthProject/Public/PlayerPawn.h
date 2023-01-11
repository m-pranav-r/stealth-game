// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include "EnhancedInput/Public/InputActionValue.h"
//#include "Components/StaticMeshComponent.h"
#include "PlayerPawn.generated.h"

struct FInputActionValue;

UCLASS()
class APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	class UStaticMeshComponent* MainMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	void Jump(const FInputActionValue& Value);

	void Move(const FInputActionValue& Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
