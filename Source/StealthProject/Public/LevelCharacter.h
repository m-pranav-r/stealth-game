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

	UPROPERTY(EditAnywhere, Category = "Custom Mesh")
		class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(EditAnywhere, Category = "Custom Collision")
		class USphereComponent* PlayerCollideSphere;

	UPROPERTY(EditAnywhere, Category = "Custom Pawn Sensing")
		class UPawnSensingComponent* PawnSense;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	USceneComponent* Patrol_1;
	
	UPROPERTY(EditAnywhere, Category = "Patrol")
	USceneComponent* Patrol_2;

	class APlayerCharacter* PlayerRef;

	class AEnemyAIController* EnemyAIController;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolDelay;

	FTimerHandle PatrolHandle;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	void IntermediaryWait();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ReactToAlert(APawn* PlayerPawn);

	/*UFUNCTION()
	void ClearTimerOnAlert();*/

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TArray <FVector> PatrolLocations;

	uint8 CurrentLoc = 0;
};