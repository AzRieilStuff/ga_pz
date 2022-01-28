// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerBase.generated.h"

class AArcher;

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

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintReadOnly)
	AArcher* PlayerCharacter;
	
	virtual void ClientRestart_Implementation(APawn* NewPawn) override;
};
