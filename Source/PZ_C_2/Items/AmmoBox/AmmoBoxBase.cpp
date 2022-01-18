// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBoxBase.h"

#include "PZ_C_2/Ammo/Arrow.h"

AAmmoBoxBase::AAmmoBoxBase()
{
	bDestroyOnPickup = true;
	bStoreable = true;

	ProjectileType = AArrow::StaticClass();
	AmmoAmount = 50;
}

void AAmmoBoxBase::GenerateInventoryData(FInventoryItem& InventoryData) const
{
	Super::GenerateInventoryData(InventoryData);

	InventoryData.IconLabel = FString::FromInt(AmmoAmount);
	InventoryData.Amount = AmmoAmount;
}
