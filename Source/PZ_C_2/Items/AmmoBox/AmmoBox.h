// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/BaseItem.h"
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "AmmoBox.generated.h"

class UAmmoBoxInventoryItem;
/**
 * 
 */
UCLASS()
class PZ_C_2_API AAmmoBox : public ABaseItem
{
	GENERATED_BODY()

public:
	AAmmoBox();

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoAmount;

	virtual UBaseInventoryItem* GenerateInventoryData(UBaseInventoryItem* Target = nullptr) const override;
};
