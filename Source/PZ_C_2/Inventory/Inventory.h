// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Inventory.generated.h"

/**
 * 
 */

UCLASS(BlueprintType)
class PZ_C_2_API UInventory : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventory();

	UPROPERTY()
	TArray<UInventoryItem*> Items;

	UPROPERTY(BlueprintReadWrite)
	int32 Slots = 10;

	UFUNCTION(BlueprintCallable)
	TArray<UInventoryItem*> GetItems() const;

	UFUNCTION(BlueprintCallable)
	UInventoryItem* GetItem(const int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool AddItem(UInventoryItem* Item);

	UFUNCTION(BlueprintCallable)
	bool HasFreeSlot();
};
