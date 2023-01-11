// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputConfigData.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//Mesh Setup
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);

	//Camera Setup
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(MainMesh);
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(MainMesh);
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->TargetArmLength = 300.0f;
}

void APlayerPawn::Jump(const FInputActionValue& Value)
{
	SetActorLocation(GetActorLocation() + FVector(1.0f ,1.0f, 1.0f));
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Move Called."));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Input setup initiated."));

	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UE_LOG(LogTemp, Warning, TEXT("Subsystem setup."));

	UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EI->BindAction(InputActions->IA_Jump, ETriggerEvent::Triggered, this, &APlayerPawn::Jump);
	EI->BindAction(InputActions->IA_Move, ETriggerEvent::Triggered, this, &APlayerPawn::Move);

	UE_LOG(LogTemp, Warning, TEXT("Bindings setup."));
}

