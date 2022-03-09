// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EInventorySlot : uint8
{
	None = 0,
	MainWeaponAmmo,
	Quiver,
	MainWeapon,
	Consumable,
	MAX
};

ENUM_RANGE_BY_FIRST_AND_LAST(EInventorySlot, EInventorySlot::None, EInventorySlot::MAX);

class ABaseItem;
class UBaseInventoryItem;
class AArcher;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteraction, const UBaseInventoryItem*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemStateChange, UBaseInventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotItemChanged, const EInventorySlot, Slot);

/**
 * 
*/
UCLASS(BlueprintType)
class PZ_C_2_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	// Active items for each slot
	UPROPERTY()
	TMap<EInventorySlot, UBaseInventoryItem*> ActiveItems;

	// Active items for each slot
	TMap<EInventorySlot, int32> DistinctItemLimits;

	// All items
	UPROPERTY()
	TArray<UBaseInventoryItem*> Items;

	// todo Switching delay
	TMap<EInventorySlot, float> SwitchDelay;

	// todo Activation delay
	TMap<EInventorySlot, float> ActivationDelay;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TMap<TSubclassOf<ABaseItem>, int32> DefaultInventory;

	void InitDefaultInventory();
public:
	UInventoryManagerComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	inline TMap<EInventorySlot, UBaseInventoryItem*> GetActiveItems() const { return ActiveItems; };

	TArray<UBaseInventoryItem*> GetItems(const EInventorySlot SlotType) const;

	UBaseInventoryItem* GetActiveItem(const EInventorySlot SlotType) const;

#pragma region Adding
	UPROPERTY(BlueprintAssignable)
	FOnItemStateChange OnItemPicked;

	UPROPERTY(BlueprintAssignable)
	FOnSlotItemChanged OnActiveSlotItemChanged;

	// Any of inventory contains was changed ( items, amount, etc )
	//FOnInventoryStateChange OnInventoryStateChange;

	// [server + client] add item to inventory if possible ( affect local only )
	bool TryAddItem(UBaseInventoryItem* Item);

	bool TryAddItem(ABaseItem* ItemActor);

	bool CanStoreItem(const UBaseInventoryItem* Item) const;

	UFUNCTION()
	void UpdateSelectedItem(UBaseInventoryItem* UpdatedItem);

	void UpdateSelectedItem(const EInventorySlot Slot);

	UFUNCTION()
	void RemoveItem(UBaseInventoryItem* Item);

#pragma endregion
#pragma region Using
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemStateChange OnItemAmountChange;

	UPROPERTY(BlueprintAssignable)
	FOnItemStateChange OnItemRemoved;

	void ConsumeItem(const EInventorySlot ActiveSlot, const int32 Amount);

	void ConsumeItem(UBaseInventoryItem* Item, const int32 Amount);

#pragma endregion
};
