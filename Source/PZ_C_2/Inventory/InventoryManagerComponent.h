// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteraction, const UBaseInventoryItem*, Item);

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
public:
	UInventoryManagerComponent();

	virtual void BeginPlay() override;

	TArray<UBaseInventoryItem*> GetItems(const EInventorySlot SlotType) const;

	UBaseInventoryItem* GetActiveItem(const EInventorySlot SlotType) const;

	bool TryAddItem(UBaseInventoryItem* Item);

	bool TryAddItem(ABaseItem* ItemActor);

	bool CanStoreItem(const UBaseInventoryItem* Item) const;
};
