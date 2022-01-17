// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBow.h"
#include "ShortgunBow.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API AShortgunBow : public ABaseBow
{
	GENERATED_BODY()

	virtual void ServerPerformFire_Implementation(FVector AimLocation) override;

public:
	AShortgunBow();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ProjectilesAmount;

	virtual void ComputeProjectileTransform(const AArcher* Character, FVector AimLocation, FVector& Location, FRotator& Rotation) override;
};
