// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "InventoryManagerComponent.generated.h"

UENUM()
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemStateChange, const UBaseInventoryItem*, Item);

DECLARE_MULTICAST_DELEGATE(FOnInventoryStateChange)

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

	const TArray<UBaseInventoryItem*> GetItems(const EInventorySlot SlotType) const;

	const UBaseInventoryItem* GetActiveItem(const EInventorySlot SlotType) const;

#pragma region Adding
	UPROPERTY(BlueprintAssignable)
	FOnItemStateChange OnItemPicked;

	// Any of inventory contains was changed ( items, amount, etc )
	FOnInventoryStateChange OnInventoryStateChange;

	// [server + client] add item to inventory if possible ( affect local only )
	bool TryAddItem(UBaseInventoryItem* Item);

	bool TryAddItem(ABaseItem* ItemActor);

	bool CanStoreItem(const UBaseInventoryItem* Item) const;

	void UpdateSelectedItem();
#pragma endregion
#pragma region Using
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemStateChange OnItemAmountChange;

	void ConsumeItem(EInventorySlot ActiveSlot, const int32 Amount);

	void ConsumeItem(const UBaseInventoryItem* Item, const int32 Amount);
#pragma endregion
};
