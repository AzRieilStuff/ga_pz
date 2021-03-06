// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include "PZ_C_2/Items/Core/PickBoxComponent.h"

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

void UInventoryManagerComponent::SetItemAmount(const int32 ItemIndex, const int32 Amount)
{
	UBaseInventoryItem* Item = GetItem(ItemIndex);
	check(Item);
	Item->Amount = Amount;

	if (Item->Amount <= 0)
	{
		Items.RemoveAt(ItemIndex);
		OnItemRemoved.Broadcast(Item);
	}
}

void UInventoryManagerComponent::OnUseItemAction(const int32 ItemIndex)
{
	UBaseInventoryItem* Item = GetItem(ItemIndex);
	if (Item == nullptr || Item->IsLocked())
	{
		return;
	}

	Item->Lock();
	ServerUseItem(ItemIndex);
}

void UInventoryManagerComponent::ServerUseItem_Implementation(const int32 ItemIndex)
{
	UBaseInventoryItem* Item = GetItem(ItemIndex);
	if (Item == nullptr)
	{
		return;
	}

	AArcher* Character = Cast<AArcher>(GetOwner());

	if (!Item->CanUsedBy(Character) || !Item->CanUsedOn(Character))
	{
		return;
	}

	const bool Used = Item->UseItem(Character);

	if (Used)
	{
		SetItemAmount(ItemIndex, Item->Amount - 1);
	}

	if (!Character->IsLocallyControlled())
	{
		ClientUseItem(ItemIndex, Used);
	}
	else
	{
		Item->Unlock();
	}
}

void UInventoryManagerComponent::ClientUseItem_Implementation(const int32 ItemIndex, const bool Used)
{
	UBaseInventoryItem* Item = GetItem(ItemIndex);

	if (Item == nullptr)
	{
		return;
	}

	if (Used)
	{
		SetItemAmount(ItemIndex, Item->Amount - 1);
	}

	Item->Unlock();
}

void UInventoryManagerComponent::MulticastStoreItem_Implementation(ABaseItem* Item)
{
	check(Item);

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy) // avoid duplication on listen server
	{
		UBaseInventoryItem* ItemData = Item->GenerateInventoryData();

		Items.Add(ItemData);
		OnItemStored.Broadcast(ItemData);
	}

	Item->OnStored();
}

void UInventoryManagerComponent::ServerStoreItem_Implementation(ABaseItem* Item)
{
	check(Item);

	UBaseInventoryItem* ItemData = Item->GenerateInventoryData();

	Items.Add(ItemData);
	OnItemStored.Broadcast(ItemData);

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

void UInventoryManagerComponent::OnDropItemAction()
{
	if (FocusItemIndex >= 0) // some item in hover
	{
		ServerDropItem(FocusItemIndex);
	}
}

bool UInventoryManagerComponent::ServerDropItem_Validate(const int32 ItemIndex)
{
	return ItemIndex >= 0 && ItemIndex < Items.Num(); // index valid
}

void UInventoryManagerComponent::ServerDropItem_Implementation(const int32 ItemIndex)
{
	UBaseInventoryItem* Item = GetItem(ItemIndex);
	check(Item);

	// check valid position
	FHitResult DirectTrace, LandTrace;
	FCollisionQueryParams TraceParams;

	TraceParams.AddIgnoredActor(GetOwner()); // ignore self
	TraceParams.TraceTag = FName("Debug");

	GetWorld()->LineTraceSingleByChannel(
		DirectTrace,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 150.f),
		ECC_Visibility,
		TraceParams
	);

	if (DirectTrace.bBlockingHit) // blocked by smth
	{
		return;
	}

	FVector LandTraceStart = DirectTrace.bBlockingHit ? DirectTrace.Location : DirectTrace.TraceEnd;

	GetWorld()->LineTraceSingleByChannel(
		LandTrace,
		LandTraceStart,
		LandTraceStart + (FVector::DownVector * 200.f),
		ECC_Visibility,
		TraceParams
	);

	if (!LandTrace.bBlockingHit) // too deep
	{
		return;
	}

	AArcher* Character = Cast<AArcher>(GetOwner());
	check(Character);

	// location is valid, spawn item
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(LandTraceStart);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

	ABaseItem* SpawnedItem = GetWorld()->SpawnActorDeferred<ABaseItem>(
		Item->VisualActorClass,
		SpawnTransform,
		GetOwner(),
		Character,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);

	if (SpawnedItem == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Failed to spawn");
		return;
	}

	//FVector AdjustedLocation = LandTrace.Location + SpawnedItem->GetPickBoxComponent()->GetScaledBoxExtent().Y / 2.f;
	//SpawnTransform.SetLocation(AdjustedLocation);

	SpawnedItem->OnDropped();
	SpawnedItem->FinishSpawning(SpawnTransform);
}
