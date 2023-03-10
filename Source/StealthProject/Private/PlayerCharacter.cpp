// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputConfigData.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "LevelCharacter.h"
#include "PCAnimInst.h"
#include "Kismet/GameplayStatics.h"
#include "DistCharacter.h"
#include "LevelEntity.h"
#include "StealthPlayerController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Unreal Third Person demo kenchi dobbesina code
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 700.f;				//alter all these values again in BP
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//Collision Setup
	GetCapsuleComponent()->InitCapsuleSize(100.0f, 100.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	//Camera Setup
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetArmLength = 300.0f;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AStealthPlayerController* PlayerController = Cast<AStealthPlayerController>(Controller))
	{
		//PlayerController->SetPlayer(this);
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	if (USkeletalMeshComponent* SkelMesh = this->FindComponentByClass<USkeletalMeshComponent>())
	{
		AnimInst = Cast<UPCAnimInst>(SkelMesh->GetAnimInstance());
		if (!AnimInst)
		{
			UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Cannot Initialize AnimInst"));
			return;
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Input setup initiated."));

	APlayerController* PC = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Subsystem setup."));

	UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EI->BindAction(InputActions->IA_Jump, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
	EI->BindAction(InputActions->IA_Jump, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
	EI->BindAction(InputActions->IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EI->BindAction(InputActions->IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EI->BindAction(InputActions->IA_CreateHit, ETriggerEvent::Triggered, this, &APlayerCharacter::CreateHit);
	EI->BindAction(InputActions->IA_ScaleChange, ETriggerEvent::Triggered, this, &APlayerCharacter::ChangeScale);
	EI->BindAction(InputActions->IA_Dist, ETriggerEvent::Triggered, this, &APlayerCharacter::CreateDist);
	EI->BindAction(InputActions->IA_Interact, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);

	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Bindings setup."));
}

void APlayerCharacter::SetInteractPointer(AActor* SettingActor, bool isEnemy)
{
	if (isEnemy)
	{
		InteractPointer = Cast<ALevelCharacter>(SettingActor);
		if (InteractPointer.IsValid()) UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Enemy pointer primed!"));
	}
	else
	{
		InteractPointer = Cast<ALevelEntity>(SettingActor);
		if (InteractPointer.IsValid()) UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Enemy pointer primed!"));
	}
	isInteractEnemy = isEnemy;
}

void APlayerCharacter::RemoveInteractPointer()
{
	InteractPointer.Reset();
	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Interaction pointer nulled!"));
}

void APlayerCharacter::Hit()
{
	if (Health <= 1.0f)
	{
		DisableInput(GetWorld()->GetFirstPlayerController());
		AnimInst->isGot = true;
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::Game Over!"));
	}
	Health -= HitFactor;
	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Ouch!"));
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	AnimInst->isMoving = true;
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump called."));
	ACharacter::Jump();
	AnimInst->isJumping = true;
}

void APlayerCharacter::StopJumping(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("StopJumping called."));
	ACharacter::StopJumping();
}

void APlayerCharacter::ChangeScale(const FInputActionValue& Value)
{
	if (isScaled) isScaled = false;
	else isScaled = true;
	needScaling = true;
}

void APlayerCharacter::Interact(const FInputActionValue& Value)
{
	if (!InteractPointer.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::Cannot Interact, interaction pointer not set!"));
		return;
	}
	if (isInteractEnemy)
	{
		Cast<ALevelCharacter>(InteractPointer.Get())->Interact(this);
		return;
	}
	Cast<ALevelEntity>(InteractPointer.Get())->Interact(this);
}

void APlayerCharacter::CreateHit()
{
	FHitResult Hit;
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = CameraComp->GetComponentLocation() + CameraComp->GetComponentRotation().Vector() * HitLength;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
	if (Cast<ALevelCharacter>(Hit.GetActor()))
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
		UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());
		if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::Trace hit actor: %s"), *Hit.GetActor()->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("APlayerCharacter::No Actors were hit"));
		}
	}
}

void APlayerCharacter::CreateDist()
{
	FHitResult Hit;
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = CameraComp->GetComponentLocation() + CameraComp->GetComponentRotation().Vector() * HitLength;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	if (Hit.bBlockingHit)
	{
		FVector Location = Hit.Location + FVector(0.0f, 0.0f, 5.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor<ADistCharacter>(BP_Poin, Location, Rotation, SpawnInfo);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetVelocity().Size() < StopSpeed)
	{
		AnimInst->isMoving = false;
		AnimInst->isJumping = false;
	}
	if (isScaled && needScaling)
	{
		ScalePercentage += DeltaTime * ScaleSpeed;
		this->SetActorRelativeScale3D(((1 - (ScalePercentage * ScalePercentage)) * FVector(1.0f, 1.0f, 1.0f)) + (ScalePercentage * ScalePercentage) * ScaleFactor);
	}
	if (ScalePercentage >= 1.0f)
	{
		needScaling = false;
		ScalePercentage = 0.f;
	}
	if ((!isScaled) && (needScaling))
	{
		ScalePercentage += DeltaTime * ScaleSpeed;
		this->SetActorRelativeScale3D(((1 - (ScalePercentage * ScalePercentage)) * ScaleFactor) + (ScalePercentage * ScalePercentage) * FVector(1.0f, 1.0f, 1.0f));
	}
}