// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryManagerComponent.generated.h"

class ABaseItem;
class UBaseInventoryItem;
class AArcher;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteraction, const UBaseInventoryItem*, Item);

/**
 * 
*/
UCLASS(BlueprintType)
class PZ_C_2_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStoreItem(ABaseItem* Item);
public:
	UInventoryManagerComponent();

	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<UBaseInventoryItem*> Items;

	UPROPERTY(BlueprintReadWrite)
	int32 Slots = 10;

	UFUNCTION(BlueprintCallable)
	TArray<UBaseInventoryItem*> GetItems();

	UFUNCTION(BlueprintCallable)
	UBaseInventoryItem* GetItem(const int32 Index) const;

	void SetItemAmount(const int32 ItemIndex, const int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void OnUseItemAction(const int32 ItemIndex);
	
	UFUNCTION(Server, BlueprintCallable, Reliable) /**/
	virtual void ServerUseItem(const int32 ItemIndex);
	
	UFUNCTION(Client, Reliable)
	virtual void ClientUseItem(const int32 ItemIndex, const bool Used);

	UFUNCTION(Server, BlueprintCallable, Reliable)
	void ServerStoreItem(ABaseItem* Item);

	UFUNCTION(BlueprintCallable)
	bool HasFreeSlot() const;

	bool CanPickupItem(const class ABaseItem* Item) const;

	UPROPERTY(BlueprintAssignable)
	FOnItemInteraction OnItemRemoved;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemInteraction OnItemStored;

	// [local] changes from local ui`
	UPROPERTY(BlueprintReadWrite)
	int32 FocusItemIndex;

	void OnDropItemAction();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropItem(const int ItemIndex);
};
