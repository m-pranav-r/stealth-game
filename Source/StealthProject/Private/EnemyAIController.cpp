// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "NavigationSystem.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	//RandomPatrol();
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
