// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Arrow.generated.h"

UCLASS()
class PZ_C_2_API AArrow : public ABaseProjectile
{
	GENERATED_BODY()

	FTimerHandle DestroyTimer;

	float Lifetime = 150.f;

public:
	// Sets default values for this actor's properties
	AArrow();

	virtual void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                                const FHitResult& Hit) override;

	// [server]
	virtual void OnShoot() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
