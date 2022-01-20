// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryManagerComponent.generated.h"

class ABaseItem;
class UBaseInventoryItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteraction, const ABaseItem*, Item);

/**
 * 
*/
UCLASS(BlueprintType)
class PZ_C_2_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION(Client, Reliable)
	void ClientStoreItem(const ABaseItem* Item, UBaseInventoryItem* ItemData);
public:
	UInventoryManagerComponent();

	UPROPERTY()
	TArray<UBaseInventoryItem*> Items;

	UPROPERTY(BlueprintReadWrite)
	int32 Slots = 10;

	UFUNCTION(BlueprintCallable)
	TArray<UBaseInventoryItem*> GetItems();

	UFUNCTION(BlueprintCallable)
	UBaseInventoryItem* GetItem(const int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool UseItem(const int32 ItemIndex);

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void ServerStoreItem(const ABaseItem* Item);

	UFUNCTION(BlueprintCallable)
	bool HasFreeSlot() const;

	bool CanPickupItem(const class ABaseItem* Item) const;

	UPROPERTY(BlueprintAssignable)
	FOnItemInteraction OnItemRemoved;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemInteraction OnItemStored;
};
