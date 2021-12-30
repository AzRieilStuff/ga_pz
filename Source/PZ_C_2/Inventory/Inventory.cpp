// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

void UInventory::AddItemClient_Implementation(FInventoryItem NewItem)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Add item client"));
	Items.Add(NewItem);
}

UInventory::UInventory()
{
}

TArray<FInventoryItem>& UInventory::GetItems()
{
	return Items;
}

bool UInventory::GetItem(const int32 Index, FInventoryItem& Out) const
{
	if (Index >= Items.Num())
	{
		Out = Items[Index];
		return true;
	}
	return false;
}

void UInventory::AddItemServer_Implementation(FInventoryItem Item)
{
	AddItemClient(Item);
}

bool UInventory::HasFreeSlot()
{
	return Items.Num() < Slots;
}
