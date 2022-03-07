#include "AHealthKit.h"

#include "PZ_C_2/Characters/Archer.h"


AHealthKit::AHealthKit()
{
	HealAmount = 10;
	bDestroyOnPickup = true;
	bPickable = true;
	bStackable = true;
}

bool AHealthKit::CanPickupBy(AArcher* Character) const
{
	return Super::CanPickupBy(Character);
}
