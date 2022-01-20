// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"

#include "PZ_C_2/Ammo/Arrow.h"


bool UAmmoBoxInventoryItem::UseItem(AArcher* Target)
{
	return Super::UseItem(Target);
}

AAmmoBox::AAmmoBox()
{
	bDestroyOnPickup = true;
	bStoreable = true;

	ProjectileType = AArrow::StaticClass();
	AmmoAmount = 50;
}

UAmmoBoxInventoryItem* AAmmoBox::GenerateInventoryData(UBaseInventoryItem* Target) const
{
	UAmmoBoxInventoryItem* AmmoItem = NewObject<UAmmoBoxInventoryItem>();
	Super::GenerateInventoryData(AmmoItem);

	AmmoItem->IconLabel = FString::FromInt(AmmoAmount);
	AmmoItem->Amount = AmmoAmount;

	return AmmoItem;
}
