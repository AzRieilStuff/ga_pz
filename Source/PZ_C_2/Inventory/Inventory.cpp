// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

UInventory::UInventory()
{
}

TArray<UInventoryItem*>& UInventory::GetItems()
{
	return Items;
}

UInventoryItem* UInventory::GetItem(const int32 Index) const
{
	return Index >= Items.Num() ? nullptr : Items[Index];
}

bool UInventory::AddItem(UInventoryItem* Item)
{
	Items.Add(Item);
	return true;
}

bool UInventory::HasFreeSlot()
{
	return Items.Num() < Slots;
}
