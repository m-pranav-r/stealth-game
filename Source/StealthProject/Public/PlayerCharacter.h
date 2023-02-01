// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerAlertEventSignature, FVector, AlertLocation)

struct FInputActionValue;

UCLASS()
class STEALTHPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	FPlayerAlertEventSignature PlayerAlertEvent_OnCreate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Skeletal Mesh")
	USkeletalMesh* MainMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Enhanced Input")
	class UInputConfigData* InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Collision")
	float HitLength = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Movement")
	float StopSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Movement")
	FVector ScaleFactor = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Movement")
	float ScaleSpeed = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Custom Mechanics")
	float HitFactor = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Mechanics")
	TSubclassOf<AActor> BP_Poin;

	TWeakObjectPtr<AActor>InteractPointer;

	bool isScaled = false;

	bool needScaling = false;

	bool needTeleport = false;

	bool isTeleporting = false;

	bool isInteractEnemy = false;

	float ScalePercentage = 0.0f;

	float Health = 100.0f;

	class UPCAnimInst* AnimInst;

	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void StopJumping(const FInputActionValue& Value);

	void ChangeScale(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void CreateHit();

	void CreateDist();

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetInteractPointer(AActor* SettingActor, bool isEnemy);

	void RemoveInteractPointer();

	void Hit();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArmComp; }
	
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }

};
