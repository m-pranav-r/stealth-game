// Fill out your copyright notice in the Description page of Project Settings.


#include "ParkourEntity.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"

// Sets default values
AParkourEntity::AParkourEntity()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(320.f, 120.f, 120.f));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetGenerateOverlapEvents(true);

	BlockComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockComponent"));
	BlockComponent->SetBoxExtent(FVector(300.f, 100.f, 100.f));
	BlockComponent->SetupAttachment(RootComponent);
	BlockComponent->SetGenerateOverlapEvents(false);
	BlockComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlockComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BlockComponent->SetupAttachment(RootComponent);

	TeleportZone = CreateDefaultSubobject<USceneComponent>(TEXT("TeleportZone"));
	TeleportZone->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AParkourEntity::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AParkourEntity::OverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AParkourEntity::OverlapEnd);
	TeleportLocation = TeleportZone->GetComponentLocation();
}

void AParkourEntity::OverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("AParkourEntity::Overlap with player started."));
	}
	else return;
	PlayerChar->SetInteractPointer(this, 0);
}

void AParkourEntity::OverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("AParkourEntity::Overlap with player ended."));
	}
	else return;
	PlayerChar->RemoveInteractPointer();
}

// Called every frame
void AParkourEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParkourEntity::Interact(AActor* CallingActor)
{
	CallingActor->SetActorLocation(TeleportLocation);
	UE_LOG(LogTemp, Warning, TEXT("AParkourEntity::Player Location changed!"));
}
