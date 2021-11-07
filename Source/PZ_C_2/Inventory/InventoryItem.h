// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class PZ_C_2_API UInventoryItem : public UObject
{
	GENERATED_BODY()
public:
	UInventoryItem();
	UInventoryItem(FName Name);

	UPROPERTY(BlueprintReadWrite)
	FName Name;
};
