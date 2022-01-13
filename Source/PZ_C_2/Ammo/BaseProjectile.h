// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class PZ_C_2_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABaseProjectile();

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                        FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly)
	bool bDestroyOnHit;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	class UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* OnHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Damage")
	float Damage;
};
