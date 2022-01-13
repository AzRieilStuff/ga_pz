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

	virtual void PerformFire() override;

public:
	AShortgunBow();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ProjectilesAmount;
};
