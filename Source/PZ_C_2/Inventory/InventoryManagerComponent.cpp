// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "PZ_C_2/Items/Core/BaseItem.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
}

void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->GetLocalRole() > ROLE_SimulatedProxy)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, GetOwner()->GetName());
	}
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

	if (!Item->CanUsedBy(Character) || !Item->CanUsedOn(Character))
	{
		return false;
	}

	Item->UseItem(Character);
	return true;
}

void UInventoryManagerComponent::MulticastStoreItem_Implementation(ABaseItem* Item)
{
	check(Item);

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) // avoid duplication on listen server
	{
		UBaseInventoryItem* ItemData = Item->GenerateInventoryData();

		Items.Add(ItemData);
		OnItemStored.Broadcast(Item);
	}

	Item->OnStored();
}

void UInventoryManagerComponent::ServerStoreItem_Implementation(ABaseItem* Item)
{
	check(Item);

	UBaseInventoryItem* ItemData = Item->GenerateInventoryData();

	Items.Add(ItemData);
	OnItemStored.Broadcast(Item);

	MulticastStoreItem(Item);
}

bool UInventoryManagerComponent::HasFreeSlot() const
{
	return Items.Num() < Slots;
}

bool UInventoryManagerComponent::CanPickupItem(const ABaseItem* Item) const
{
	return HasFreeSlot();
}
