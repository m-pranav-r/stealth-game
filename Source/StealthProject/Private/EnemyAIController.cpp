// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "LevelCharacter.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	//DefaultPatrol(0);
}

void AEnemyAIController::RandomPatrol()
{
	if (!NavArea)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController::Couldn't find NavArea."));
		return;
	}
	NavArea->K2_GetRandomPointInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 3000.0f);
	MoveToLocation(RandomLocation);
}

void AEnemyAIController::DefaultPatrol(uint8 LocIndex)
{
	if (!NavArea)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEnemyAIController::Couldn't find NavArea."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Request to move to location %i."), LocIndex);
	ALevelCharacter* AIPawn = Cast<ALevelCharacter>(GetPawn());
	MoveToLocation(AIPawn->PatrolLocations[LocIndex]);
}
