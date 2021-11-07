// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	APlayerCharacter::InitInventory();
}


UInventory* APlayerCharacter::GetInventory()
{
	return InventoryComponent;
}

void APlayerCharacter::InitInventory()
{
	InventoryComponent = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
}
