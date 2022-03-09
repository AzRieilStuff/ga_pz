// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "Containers/ContainerAllocationPolicies.h"
#include "PZ_C_2/Items/Core/BaseItem.h"

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
		UBaseInventoryItem* Item = Row.Key.GetDefaultObject()->GenerateInventoryData();

		if (Item == nullptr)
		{
			UE_LOG(LogInventory, Error, TEXT("Failed to init default inventory on item %s"),
			       *Item->GetName());
			continue;
		}

		if (Item->GetIsStackable())
		{
			Item->SetAmount(Row.Value < 1 ? 1 : Row.Value);
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
	
	OnItemPicked.AddDynamic(this, &ThisClass::UpdateSelectedItem);

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
			if (SameSlotItem->GetAmount() < PerStackLimit)
			{
				// merge items to its possible cap.
				SameSlotItem->SetAmount(FMath::Min(
					SameSlotItem->GetAmount() + Item->GetAmount(),
					PerStackLimit
				));

				//OnInventoryStateChange.Broadcast();
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

	//OnInventoryStateChange.Broadcast();
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

		return !SameSlotItem->GetIsStackFull();
	}

	// check place for new unique items
	if (SameSlotItems.Num() >= DistinctItemLimits[Item->SlotType])
	{
		return false;
	}

	return true;
}

void UInventoryManagerComponent::UpdateSelectedItem(UBaseInventoryItem* UpdatedItem)
{
	UpdateSelectedItem(UpdatedItem->SlotType);
}

void UInventoryManagerComponent::UpdateSelectedItem(const EInventorySlot Slot)
{
	if (ActiveItems[Slot] != nullptr) // there is already an item
	{
		return;
	}

	TArray<UBaseInventoryItem*> CurrentSlotItems = GetItems(Slot);
	if (CurrentSlotItems.Num() == 0)
	{
		return;
	}

	ActiveItems[Slot] = CurrentSlotItems[0];
	OnActiveSlotItemChanged.Broadcast(Slot);
}

void UInventoryManagerComponent::RemoveItem(UBaseInventoryItem* Item)
{
	if (Item->GetAmount() > 0)
	{
		return;
	}

	if (ActiveItems[Item->SlotType] == Item)
	{
		ActiveItems[Item->SlotType] = nullptr;
		OnActiveSlotItemChanged.Broadcast(Item->SlotType);
	}

	Items.Remove(Item);
	UpdateSelectedItem(Item);

	OnItemRemoved.Broadcast(Item);
}

void UInventoryManagerComponent::ConsumeItem(const EInventorySlot ActiveSlot, const int32 Amount)
{
	return ConsumeItem(GetActiveItem(ActiveSlot), Amount);
}

void UInventoryManagerComponent::ConsumeItem(UBaseInventoryItem* Item, const int32 Amount)
{
	if (Amount == 0 || Item == nullptr)
	{
		UE_LOG(LogInventory, Warning, TEXT("Invalid consuming call"));
		return;
	}

	// ensure item is present in current inventory
	const auto InventoryItemPtr = Items.FindByPredicate([Item](UBaseInventoryItem* Value)
	{
		return Value == Item;
	});

	// passed item isnt stored in this inventory
	if (InventoryItemPtr == nullptr)
	{
		UE_LOG(LogInventory, Error, TEXT("Trying to remove %s from %s inventory, but there is no one"),
		       *Item->GetName(),
		       *GetOwner()->GetName())
		return;
	}

	UBaseInventoryItem* InventoryItem = *InventoryItemPtr;
	InventoryItem->ModifyAmount(-Amount);
	
	if (Amount <= 0)
	{
		RemoveItem(InventoryItem);
	}
	else
	{
		OnItemAmountChange.Broadcast(InventoryItem);

		if( ActiveItems[InventoryItem->SlotType] == InventoryItem )
		{
			OnActiveSlotItemChanged.Broadcast(InventoryItem->SlotType);
		}
	}
}
