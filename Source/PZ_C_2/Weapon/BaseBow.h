#pragma once

#include "BaseRangeWeapon.h"
#include "PZ_C_2/Ammo/Arrow.h"
#include "BaseBow.generated.h"

UCLASS(Config=Weapon, defaultconfig)
class ABaseBow : public ABaseRangeWeapon
{
	GENERATED_BODY()

	const FName ArrowSocketName = FName("ArrowSocket");
	
protected:
	virtual class ABaseProjectile* SpawnProjectile(FVector AimLocation) override;

virtual void ComputeProjectileTransform(const AArcher* Character, FVector AimLocation, FVector& Location, FRotator& Rotation) override;
public:
	ABaseBow();

};
