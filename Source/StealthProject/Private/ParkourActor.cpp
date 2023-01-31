// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourActor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

// Sets default values
AParkourActor::AParkourActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TeleportZone = CreateDefaultSubobject<USceneComponent>(TEXT("TeleportZone"));
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AParkourActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AParkourActor::OverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AParkourActor::OverlapEnd);
}

void AParkourActor::OverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap with player started."));
	}
}

void AParkourActor::OverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap with player ended."));
	}
}

// Called every frame
void AParkourActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}