// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBoxInventoryItem.h"

UAmmoBoxInventoryItem::UAmmoBoxInventoryItem()
{
	SlotType = EInventorySlot::MainWeaponAmmo;
}

int32 UAmmoBoxInventoryItem::GetStackLimit() const
{
	return 50;
}
