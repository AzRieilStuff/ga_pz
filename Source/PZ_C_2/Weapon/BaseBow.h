﻿#pragma once

#include "BaseRangeWeapon.h"
#include "PZ_C_2/Ammo/Arrow.h"
#include "BaseBow.generated.h"

UCLASS()
class ABaseBow : public ABaseRangeWeapon
{
	GENERATED_BODY()

	const FName ArrowSocketName = FName("ArrowSocket");
	
protected:
	virtual class ABaseProjectile* SpawnProjectile() override;

public:
	ABaseBow();

	virtual void PickupMulticast_Implementation(AArcher* Character) override;
};
