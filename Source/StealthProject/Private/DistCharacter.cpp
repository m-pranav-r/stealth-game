// Fill out your copyright notice in the Description page of Project Settings.


#include "DistCharacter.h"

// Sets default values
ADistCharacter::ADistCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADistCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ADistCharacter::Spawned at %s"), *GetActorLocation().ToString());
	
}

// Called every frame
void ADistCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADistCharacter::DestroyExternal()
{
	UE_LOG(LogTemp, Warning, TEXT("ADistActor::Distraction Destroyed!"));
	this->Destroy();
}

// Called to bind functionality to input
void ADistCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

