// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Perception/PawnSensingComponent.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.h"

// Sets default values
ALevelCharacter::ALevelCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMeshMain"));
	EnemyMesh->SetupAttachment(RootComponent);
	PlayerCollideSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerCollideSphere"));
	PlayerCollideSphere->SetupAttachment(RootComponent);
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SetPeripheralVisionAngle(90.0f);
}

// Called when the game starts or when spawned
void ALevelCharacter::BeginPlay()
{
	Super::BeginPlay();
	EnemyAIController = Cast<AEnemyAIController>(GetController());
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ALevelCharacter::OnAIMoveCompleted);
	PawnSense->OnSeePawn.AddDynamic(this, &ALevelCharacter::ReactToAlert);
}

void ALevelCharacter::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	EnemyAIController->RandomPatrol();
}

// Called every frame
void ALevelCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALevelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALevelCharacter::ReactToAlert(APawn* PlayerPawn)
{
	AEnemyAIController* AIC = Cast<AEnemyAIController>(GetController());
	if (AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Player Caught!."));
	}
}

