// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseItem.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
}

TArray<UBaseInventoryItem*> UInventoryManagerComponent::GetItems()
{
	return Items;
}

UBaseInventoryItem* UInventoryManagerComponent::GetItem(const int32 Index) const
{
	if (Index < Items.Num())
	{
		return Items[Index];
	}
	return nullptr;
}

bool UInventoryManagerComponent::UseItem(const int32 ItemIndex)
{
	UBaseInventoryItem* Item = GetItem(ItemIndex);
	if (Item == nullptr)
	{
		return false;
	}

	AArcher* Character = Cast<AArcher>(GetOwner());
	const bool Result = Item->UseItem(Character);
	
	if( Result )
	{
		Item->Amount -= 1;

		if( Item->Amount <= 0 )
		{
			///
		}
	}

	return Result;
}

void UInventoryManagerComponent::ClientStoreItem_Implementation(const ABaseItem* Item, UBaseInventoryItem* ItemData)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Items.Add(ItemData);
		OnItemStored.Broadcast(Item);
	}
}

void UInventoryManagerComponent::ServerStoreItem_Implementation(const ABaseItem* Item)
{
	check(Item);

	UBaseInventoryItem* ItemData = Item->GenerateInventoryData();

	Items.Add(ItemData);
	OnItemStored.Broadcast(Item);

	ClientStoreItem(Item, ItemData);
}

bool UInventoryManagerComponent::HasFreeSlot() const
{
	return Items.Num() < Slots;
}

bool UInventoryManagerComponent::CanPickupItem(const ABaseItem* Item) const
{
	return HasFreeSlot();
}
