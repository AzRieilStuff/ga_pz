// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Platforms.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API UPlatforms : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static bool IsAndroid();
};
