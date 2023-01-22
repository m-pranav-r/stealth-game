// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class STEALTHPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	class UStaticMeshComponent* MainMesh;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	class UBoxComponent* BoxMesh;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	/*UPROPERTY(EditAnywhere, Category = "Meshes|Extents")
		float Extent = 40.0f;*/

	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void StopJumping(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArmComp; }
	
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }

};
