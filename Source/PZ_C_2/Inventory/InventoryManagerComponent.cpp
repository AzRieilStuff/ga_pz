// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseItem.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
}

TArray<FInventoryItem>& UInventoryManagerComponent::GetItems()
{
	return Items;
}

bool UInventoryManagerComponent::GetItem(const int32 Index, FInventoryItem& Out) const
{
	if (Index < Items.Num())
	{
		Out = Items[Index];
		return true;
	}
	return false;
}

bool UInventoryManagerComponent::UseItem(const int32 ItemIndex)
{
	FInventoryItem ItemData;
	if (!GetItem(ItemIndex, ItemData))
	{
		return false;
	}
	check(ItemData.ItemClass);

	ABaseItem* Spawned = Cast<ABaseItem>(GetWorld()->SpawnActor(ItemData.ItemClass.Get()));
	if( Spawned == nullptr )	
	{
		// ?
		return false;
	}

	AArcher* Character = Cast<AArcher>(GetOwner());

	Spawned->SetHidden(true);
	const bool Result = Spawned->UseItem(Character);
	
	if( Result )
	{
		ItemData.Amount -= 1;

		if( ItemData.Amount <= 0 )
		{
			///
		}
	}

	return Result;
}

void UInventoryManagerComponent::ClientStoreItem_Implementation(const ABaseItem* Item, const FInventoryItem& ItemData)
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

	FInventoryItem ItemData;
	Item->GenerateInventoryData(ItemData);

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
