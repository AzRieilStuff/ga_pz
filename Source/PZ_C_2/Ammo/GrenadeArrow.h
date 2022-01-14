// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Arrow.h"
#include "GrenadeArrow.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API AGrenadeArrow : public AArrow
{
	GENERATED_BODY()

public:

	AGrenadeArrow();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageRadius;
	
	virtual void ApplyDamage(AActor* Actor, FVector Origin, const FHitResult& Hit) override;
};
