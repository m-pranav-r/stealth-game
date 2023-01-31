// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Perception/PawnSensingComponent.h"
#include "PlayerCharacter.h"
#include "DistCharacter.h"
#include "EnemyAIController.h"

ALevelCharacter::ALevelCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMeshMain"));
	EnemyMesh->SetupAttachment(RootComponent);
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SetPeripheralVisionAngle(90.0f);
	
	Patrol_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Patrol1"));
	Patrol_1->SetupAttachment(RootComponent);
	Patrol_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Patrol2"));
	Patrol_2->SetupAttachment(RootComponent);
}

void ALevelCharacter::BeginPlay()
{
	Super::BeginPlay();
	PatrolLocations.Add(Patrol_1->GetComponentLocation());
	PatrolLocations.Add(Patrol_2->GetComponentLocation());
	EnemyAIController = Cast<AEnemyAIController>(GetController());
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ALevelCharacter::OnAIMoveCompleted);
	PawnSense->OnSeePawn.AddDynamic(this, &ALevelCharacter::ReactToAlert);
	EnemyAIController->DefaultPatrol(0);
}

void ALevelCharacter::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (isOccupied) return;
	GetWorldTimerManager().SetTimer(PatrolHandle, this, &ALevelCharacter::IntermediaryWait, PatrolDelay, false);
}

void ALevelCharacter::IntermediaryWait()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyActor::IntermediaryWait() called!"));
	EnemyAIController->DefaultPatrol(CurrentLoc);
	CurrentLoc = 1 - CurrentLoc;
}

void ALevelCharacter::Investigate()
{
	EnemyAIController->MoveToLocation(DistRef->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Moving to Distraction Location."));
	GetWorldTimerManager().SetTimer(PatrolHandle, this, &ALevelCharacter::DestroyExternal, InvestigateDelay, false);
}

void ALevelCharacter::DestroyExternal()
{
	DistRef->DestroyExternal();
	UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Distraction Destroyed!"));
	isOccupied = false;
	PawnSense->SetSensingUpdatesEnabled(true);
	GetWorldTimerManager().SetTimer(PatrolHandle, this, &ALevelCharacter::IntermediaryWait, ResetDelay, false);
}

void ALevelCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALevelCharacter::ReactToAlert(APawn* PlayerPawn)
{
	if (isOccupied) return;
	GetWorldTimerManager().ClearTimer(PatrolHandle);
	EnemyAIController->StopMovement();
	AEnemyAIController* AIC = Cast<AEnemyAIController>(GetController());
	if (AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Pawn Caught!."));
	}
	APlayerCharacter* PC = Cast<APlayerCharacter>(PlayerPawn);
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Player Caught!."));
		//Actual alert function
	}
	ADistCharacter* DA = Cast<ADistCharacter>(PlayerPawn);
	if (DA)
	{
		DistRef = DA;
		GetWorldTimerManager().ClearTimer(PatrolHandle);
		isOccupied = true;
		PawnSense->SetSensingUpdatesEnabled(false);
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Distraction Caught!."));
		Investigate();
		return;
	}
	GetWorldTimerManager().SetTimer(PatrolHandle, this, &ALevelCharacter::IntermediaryWait, ResetDelay, false);
}

