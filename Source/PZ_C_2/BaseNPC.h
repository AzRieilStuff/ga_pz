// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseNPC.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API ABaseNPC : public ABaseCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;
};
