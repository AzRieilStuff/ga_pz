// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

UInventory::UInventory()
{
}

TArray<UInventoryItem*> UInventory::GetItems() const
{
	return Items;
}

UInventoryItem* UInventory::GetItem(int32 index) const
{
	return index >= this->Items.Num() ? nullptr : this->Items[index];
}

bool UInventory::AddItem(UInventoryItem* item)
{
		this->Items.Add(item);
		return true;
}

bool UInventory::HasFreeSlot()
{
	return this->Items.Num() < this->Slots;
}
