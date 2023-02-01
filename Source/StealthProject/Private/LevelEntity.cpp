// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEntity.h"

// Sets default values
ALevelEntity::ALevelEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelEntity::Interact(AActor* CallingActor)
{

}

