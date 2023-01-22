// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourActor.h"

// Sets default values
AParkourActor::AParkourActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AParkourActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParkourActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}