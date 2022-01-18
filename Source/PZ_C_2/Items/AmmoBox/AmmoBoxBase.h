// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/BaseItem.h"
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"
#include "AmmoBoxBase.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API AAmmoBoxBase : public ABaseItem
{
	GENERATED_BODY()
	
public:
AAmmoBoxBase();
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoAmount;

virtual void GenerateInventoryData(FInventoryItem& InventoryData) const override;
};
