// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerControllerBase();

	virtual void BeginPlay() override;
};
