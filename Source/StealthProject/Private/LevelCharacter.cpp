// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Perception/PawnSensingComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DistCharacter.h"
#include "PCAnimInst.h"
#include "EnemyAIController.h"

ALevelCharacter::ALevelCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::Initialization started."));
	PrimaryActorTick.bCanEverTick = false;
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSense"));
	PawnSense->SetPeripheralVisionAngle(90.0f);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyBoxComponent"));
	BoxComponent->SetBoxExtent(FVector(120.f, 120.f, 120.f));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetGenerateOverlapEvents(true);
	
	Patrol_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Patrol1"));
	Patrol_1->SetupAttachment(RootComponent);
	Patrol_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Patrol2"));
	Patrol_2->SetupAttachment(RootComponent);
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::Initialization finished!"));
}

void ALevelCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::BeginPlay started."));
	PatrolLocations.Add(Patrol_1->GetComponentLocation());
	PatrolLocations.Add(Patrol_2->GetComponentLocation());
	EnemyAIController = Cast<AEnemyAIController>(GetController());
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ALevelCharacter::OnAIMoveCompleted);
	PawnSense->OnSeePawn.AddDynamic(this, &ALevelCharacter::ReactToAlert);
	EnemyAIController->DefaultPatrol(0);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelCharacter::OverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALevelCharacter::OverlapEnd);
	if (USkeletalMeshComponent* SkelMesh = this->FindComponentByClass<USkeletalMeshComponent>())
	{
		AnimInst = Cast<UPCAnimInst>(SkelMesh->GetAnimInstance());
		if (!AnimInst)
		{
			UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::Cannot Initialize AnimInst"));
			return;
		}
	}
	AnimInst->isMoving = true;
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::BeginPlay finished!"));
}

void ALevelCharacter::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (isOccupied) return;
	AnimInst->isMoving = false;
	GetWorldTimerManager().SetTimer(PatrolHandle, this, &ALevelCharacter::IntermediaryWait, PatrolDelay, false);
}

void ALevelCharacter::OverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::OverlapBegin called."));
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::Overlap with player started."));
	}
	else return;
	PlayerChar->SetInteractPointer(this, 1);
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::OverlapBegin done!"));
	playerIsInRange = true;
}

void ALevelCharacter::OverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::OverlapEnd called."));
	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor);
	if (PlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::Overlap with player ended."));
	}
	else return;
	PlayerChar->RemoveInteractPointer();
	UE_LOG(LogTemp, Warning, TEXT("ALevelCharacter::OverlapEnd done!"));
	playerIsInRange = false;
}

void ALevelCharacter::Interact(AActor* CallingActor)
{
	AnimInst->isEnemyGot = true;
}

void ALevelCharacter::IntermediaryWait()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyActor::IntermediaryWait() called!"));
	EnemyAIController->DefaultPatrol(CurrentLoc);
	AnimInst->isMoving = true;
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
	/*if (AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Pawn Caught!."));
	}*/
	APlayerCharacter* PC = Cast<APlayerCharacter>(PlayerPawn);
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Player Caught!."));
		isAlert = true;
		Chase(PC);
		return;
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


void ALevelCharacter::Chase(APlayerCharacter* Player)
{
	EnemyAIController->MoveToActor(Player);
	LastHit += UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if ((LastHit >= HitCooldown)&&(playerIsInRange))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyActor::Hit Player!"));
		Player->Hit();
		LastHit = 0;
	}
}