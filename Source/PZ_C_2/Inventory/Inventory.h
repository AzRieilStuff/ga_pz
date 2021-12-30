// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.generated.h"

/**
 * 
*/
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName Name;
};

UCLASS(BlueprintType)
class PZ_C_2_API UInventory : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION(Client, Reliable)
	void AddItemClient(FInventoryItem NewItem);
public:
	UInventory();

	UPROPERTY()
	TArray<FInventoryItem> Items;

	UPROPERTY(BlueprintReadWrite)
	int32 Slots = 10;

	UFUNCTION(BlueprintCallable)
	TArray<FInventoryItem>& GetItems();

	UFUNCTION(BlueprintCallable)
	bool GetItem(const int32 Index, FInventoryItem& Out) const;

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void AddItemServer(FInventoryItem Item);

	UFUNCTION(BlueprintCallable)
	bool HasFreeSlot();
};
