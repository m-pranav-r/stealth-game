// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LevelCharacter.generated.h"

UCLASS()
class STEALTHPROJECT_API ALevelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelCharacter();

	bool PlayerDetected;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Custom Collision")
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Custom Pawn Sensing")
		class UPawnSensingComponent* PawnSense;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	USceneComponent* Patrol_1;
	
	UPROPERTY(EditAnywhere, Category = "Patrol")
	USceneComponent* Patrol_2;

	class APlayerCharacter* PlayerRef;

	class AEnemyAIController* EnemyAIController;

	class ADistCharacter* DistRef;

	class UPCAnimInst* AnimInst;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolDelay = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float InvestigateDelay = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float ResetDelay = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float HitDistance = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float AcceptDistance = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float HitCooldown = 3.0f;

	float LastHit = 0.0f;

	FTimerHandle PatrolHandle;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	void IntermediaryWait();

	void Investigate();

	void DestroyExternal();

	bool isOccupied = false;

	bool isAlert = false;

	bool playerIsInRange = false;

	bool isGot = false;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ReactToAlert(APawn* PlayerPawn);

	void Chase(APlayerCharacter* Player);

	void Interact(AActor* CallingActor);

	/*UFUNCTION()
	void ClearTimerOnAlert();*/

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray <FVector> PatrolLocations;

	uint8 CurrentLoc = 0;
};