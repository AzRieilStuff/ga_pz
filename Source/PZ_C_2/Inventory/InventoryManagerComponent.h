// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryManagerComponent.generated.h"

class ABaseItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteraction, const ABaseItem*, Item);

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<ABaseItem> ItemClass;
	
	UPROPERTY(BlueprintReadOnly)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString IconLabel;

	UPROPERTY()
	int32 Amount;
};


/**
 * 
*/
UCLASS(BlueprintType)
class PZ_C_2_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION(Client, Reliable)
	void ClientStoreItem(const ABaseItem* Item, const FInventoryItem& ItemData);
public:
	UInventoryManagerComponent();

	UPROPERTY()
	TArray<FInventoryItem> Items;

	UPROPERTY(BlueprintReadWrite)
	int32 Slots = 10;

	UFUNCTION(BlueprintCallable)
	TArray<FInventoryItem>& GetItems();

	UFUNCTION(BlueprintCallable)
	bool GetItem(const int32 Index, FInventoryItem& Out) const;

	UFUNCTION(BlueprintCallable)
	bool UseItem(const int32 ItemIndex);

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void ServerStoreItem(const ABaseItem* Item);

	UFUNCTION(BlueprintCallable)
	bool HasFreeSlot() const;

	bool CanPickupItem(const class ABaseItem* Item) const;

	UPROPERTY(BlueprintAssignable)
	FOnItemInteraction OnItemStored;

	UPROPERTY(BlueprintAssignable)
	FOnItemInteraction OnItemRemoved;
};
