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
	
	Patrol_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Patrol1"));
	Patrol_1->SetupAttachment(RootComponent);
	Patrol_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Patrol2"));
	Patrol_2->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
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
	//EnemyAIController->RandomPatrol();
	GetWorldTimerManager().SetTimer(PatrolHandle, this, &ALevelCharacter::IntermediaryWait, PatrolDelay, false);
}

void ALevelCharacter::IntermediaryWait()
{
	EnemyAIController->DefaultPatrol(CurrentLoc);
	CurrentLoc = 1 - CurrentLoc;
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
	GetWorldTimerManager().ClearTimer(PatrolHandle);
	EnemyAIController->StopMovement();
	AEnemyAIController* AIC = Cast<AEnemyAIController>(GetController());
	if (AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Player Caught!."));
	}
	GetWorldTimerManager().SetTimer(PatrolHandle, this, &ALevelCharacter::IntermediaryWait, 10.0, false);
}

