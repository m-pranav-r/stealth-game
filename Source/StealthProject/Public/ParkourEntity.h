// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelEntity.h"
#include "ParkourEntity.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API AParkourEntity : public ALevelEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AParkourEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Mesh")
		class UStaticMeshComponent* MainMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Box")
		class UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Box")
		class UBoxComponent* BlockComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly = "Custom Mechanics")
		class USceneComponent* TeleportZone;

	FVector TeleportLocation;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OverlapEnd(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* CallingActor) override;

};