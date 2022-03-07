// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include "PZ_C_2/Items/Core/PickBoxComponent.h"

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
	OnInventoryStateChange.BindUObject(this, &ThisClass::UpdateSelectedItem);
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
		const ABaseItem* ItemClass = Item->VisualActorClass->GetDefaultObject<ABaseItem>();

		// look for similar item to merge stacks
		for (UBaseInventoryItem* SameSlotItem : SameSlotItems)
		{
			if (SameSlotItem->VisualActorClass != Item->VisualActorClass)
			{
				continue;
			}

			// if we have a similar item, try to merge them, break otherwise
			if (
				ItemClass->GetIsStackable() &&
				SameSlotItem->Amount < ItemClass->GetStackLimit())
			{
				// merge items to its possible cap.
				SameSlotItem->Amount = FMath::Min(
					SameSlotItem->Amount + Item->Amount,
					ItemClass->GetStackLimit()
				);

				OnInventoryStateChange.Execute();
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
	
	OnInventoryStateChange.Execute();
	OnItemPicked.Broadcast(Item);

	return true;
}

bool UInventoryManagerComponent::TryAddItem(ABaseItem* ItemActor)
{
	UBaseInventoryItem* NewItem = NewObject<UBaseInventoryItem>();
	return TryAddItem(ItemActor->GenerateInventoryData(NewItem));
}

bool UInventoryManagerComponent::CanStoreItem(const UBaseInventoryItem* Item) const
{
	TArray<UBaseInventoryItem*> SameSlotItems = GetItems(Item->SlotType);

	// can be added as new item
	if (SameSlotItems.Num() == 0 && DistinctItemLimits[Item->SlotType] > 0)
	{
		return true;
	}

	const ABaseItem* ItemClass = Item->VisualActorClass->GetDefaultObject<ABaseItem>();

	for (const UBaseInventoryItem* SameSlotItem : SameSlotItems)
	{
		// check can be stacked with existing item
		if (SameSlotItem->VisualActorClass != Item->VisualActorClass)
		{
			continue;
		}


		if (
			ItemClass->GetIsStackable() &&
			SameSlotItem->Amount < ItemClass->GetStackLimit())
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
