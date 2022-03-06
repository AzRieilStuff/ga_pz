// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Helpers.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API UHelpers : public UObject
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static class AArcher* GetArcherCharacter(const UWorld* WorldContext); 
};
