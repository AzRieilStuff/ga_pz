// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "Containers/ContainerAllocationPolicies.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include "PZ_C_2/Items/Core/PickBoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventory, All, All);

void UInventoryManagerComponent::InitDefaultInventory()
{
	for (const auto Row : DefaultInventory)
	{
		if (!IsValid(Row.Key))
		{
			UE_LOG(LogInventory, Warning, TEXT("Invalid default inventory record"));
			continue;
		}

		// cdo
		auto a = Row.Key.GetDefaultObject();
		UBaseInventoryItem* Item = Row.Key.GetDefaultObject()->GenerateInventoryData();

		if (Item == nullptr)
		{
			UE_LOG(LogInventory, Error, TEXT("Failed to init default inventory on item %s"),
			       *Item->GetName());
			continue;
		}

		if (Item->GetIsStackable())
		{
			Item->Amount = Row.Value < 1 ? 1 : Row.Value;
		}

		if (!TryAddItem(Item))
		{
			UE_LOG(LogInventory, Error, TEXT("Failed to add inventory item on %s"),
			       *Item->GetName());

			Item->ConditionalBeginDestroy();
		};
	}
}

UInventoryManagerComponent::UInventoryManagerComponent()
{
	ActiveItems.Empty();
	DistinctItemLimits.Empty();

	for (const EInventorySlot SlotType : TEnumRange<EInventorySlot>())
	{
		ActiveItems.Add(SlotType, nullptr);
		DistinctItemLimits.Add(SlotType, 0);
	}

	DistinctItemLimits[EInventorySlot::MainWeapon] = 1;
	DistinctItemLimits[EInventorySlot::MainWeaponAmmo] = 2;
	DistinctItemLimits[EInventorySlot::Consumable] = 2;
}

void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	OnInventoryStateChange.AddUObject(this, &ThisClass::UpdateSelectedItem);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::InitDefaultInventory);
}

TArray<UBaseInventoryItem*> UInventoryManagerComponent::GetItems(const EInventorySlot SlotType) const
{
	TArray<UBaseInventoryItem*> Result;

	for (UBaseInventoryItem* Item : Items)
	{
		if (Item->SlotType != SlotType)
		{
			continue;
		}

		Result.Add(Item);
	}

	return Result;
}

UBaseInventoryItem* UInventoryManagerComponent::GetActiveItem(EInventorySlot SlotType) const
{
	return ActiveItems[SlotType];
}

bool UInventoryManagerComponent::TryAddItem(UBaseInventoryItem* Item)
{
	TArray<UBaseInventoryItem*> SameSlotItems = GetItems(Item->SlotType);

	if (SameSlotItems.Num() > 0)
	{
		// look for similar item to merge stacks
		for (UBaseInventoryItem* SameSlotItem : SameSlotItems)
		{
			if (SameSlotItem->VisualActorClass != Item->VisualActorClass)
			{
				continue;
			}

			// if we have a similar item, try to merge them, break otherwise
			const int32 PerStackLimit = Item->GetStackLimit();
			if (SameSlotItem->Amount < PerStackLimit)
			{
				// merge items to its possible cap.
				SameSlotItem->Amount = FMath::Min(
					SameSlotItem->Amount + Item->Amount,
					PerStackLimit
				);

				OnInventoryStateChange.Broadcast();
				OnItemPicked.Broadcast(SameSlotItem);

				return true;
			}
			else
			{
				return false;
			}
		}
	}
	// check place for new unique items
	if (SameSlotItems.Num() >= DistinctItemLimits[Item->SlotType])
	{
		return false;
	}

	Items.Add(Item);

	OnInventoryStateChange.Broadcast();
	OnItemPicked.Broadcast(Item);

	return true;
}

bool UInventoryManagerComponent::TryAddItem(ABaseItem* ItemActor)
{
	//UBaseInventoryItem* NewItem = NewObject<UBaseInventoryItem>();
	return TryAddItem(ItemActor->GenerateInventoryData());
}

bool UInventoryManagerComponent::CanStoreItem(const UBaseInventoryItem* Item) const
{
	TArray<UBaseInventoryItem*> SameSlotItems = GetItems(Item->SlotType);

	// can be added as new item
	if (SameSlotItems.Num() == 0 && DistinctItemLimits[Item->SlotType] > 0)
	{
		return true;
	}

	for (const UBaseInventoryItem* SameSlotItem : SameSlotItems)
	{
		// check can be stacked with existing item
		if (SameSlotItem->VisualActorClass != Item->VisualActorClass)
		{
			continue;
		}

		if (SameSlotItem->Amount < Item->GetStackLimit())
		{
			return true;
		}
		else // limit is full
		{
			return false;
		}
	}

	// check place for new unique items
	if (SameSlotItems.Num() >= DistinctItemLimits[Item->SlotType])
	{
		return false;
	}

	return true;
}

void UInventoryManagerComponent::UpdateSelectedItem()
{
	for (const auto ActiveItem : ActiveItems)
	{
		if (ActiveItem.Value != nullptr) // there is already an item
		{
			continue;
		}

		TArray<UBaseInventoryItem*> CurrentSlotItems = GetItems(ActiveItem.Key);
		if (CurrentSlotItems.Num() == 0)
		{
			continue;
		}

		ActiveItems[ActiveItem.Key] = CurrentSlotItems[0];
	}
}

void UInventoryManagerComponent::ConsumeItem(const UBaseInventoryItem* Item, const int32 Amount)
{
	if (Amount == 0)
	{
		return;
	}
	int32 InventoryItemIndex;

	auto InventoryItem = Items.FindByPredicate([Item](UBaseInventoryItem* Value)
	{
		return Value == Item;
	});

	// passed item isnt stored in this inventory
	if (InventoryItem == nullptr)
	{
		UE_LOG(LogInventory, Error, TEXT("Trying to remove %s from %s inventory, but there is no one"), Item->GetName(),
		       GetOwner()->GetName())
		return;
	}

	(*InventoryItem)->Amount -= Amount;
	OnItemAmountChange.Broadcast(*InventoryItem);
}
