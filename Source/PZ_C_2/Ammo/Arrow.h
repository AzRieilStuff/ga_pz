// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Arrow.generated.h"

UCLASS()
class PZ_C_2_API AArrow : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArrow();

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
