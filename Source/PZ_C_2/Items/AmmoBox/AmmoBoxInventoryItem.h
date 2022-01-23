// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "UObject/Object.h"
#include "AmmoBoxInventoryItem.generated.h"

class AArcher;

/**
 * 
 */
UCLASS()
class PZ_C_2_API UAmmoBoxInventoryItem : public UBaseInventoryItem
{
	GENERATED_BODY()
public:
	UAmmoBoxInventoryItem();
};
