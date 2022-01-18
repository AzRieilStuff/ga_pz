// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include "PZ_C_2/Weapon/BaseRangeWeapon.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
}

TArray<FInventoryItem>& UInventoryManagerComponent::GetItems()
{
	return Items;
}

bool UInventoryManagerComponent::GetItem(const int32 Index, FInventoryItem Out) const
{
	if (Index >= Items.Num())
	{
		Out = Items[Index];
		return true;
	}
	return false;
}

void UInventoryManagerComponent::ClientStoreItem_Implementation(const FInventoryItem& ItemData)
{
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Add item client"));
		Items.Add(ItemData);
	}
}

void UInventoryManagerComponent::ServerStoreItem_Implementation(ABaseItem* Item)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Add item server"));

	FInventoryItem ItemData;
	Item->GenerateInventoryData(ItemData);

	Items.Add(ItemData);

	ClientStoreItem(ItemData);
}

bool UInventoryManagerComponent::HasFreeSlot() const
{
	return Items.Num() < Slots;
}

bool UInventoryManagerComponent::CanPickupItem(const ABaseItem* Item) const
{
	return HasFreeSlot();
}
