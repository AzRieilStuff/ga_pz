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
	return index >= Items.Num() ? nullptr : Items[index];
}

bool UInventory::AddItem(UInventoryItem* item)
{
	Items.Add(item);
	return true;
}

bool UInventory::HasFreeSlot()
{
	return Items.Num() < Slots;
}
