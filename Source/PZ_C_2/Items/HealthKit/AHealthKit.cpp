#include "AHealthKit.h"

#include "PZ_C_2/Characters/Archer.h"

AHealthKit::AHealthKit()
{
	HealAmount = 10;
	bDestroyOnPickup = true;
	bStoreable = true;
	bPickable = true;
}

bool AHealthKit::UseItem(AArcher* Target)
{
	Super::UseItem(Target);

	/*
	if (HealAmount >= 0 && Target->CurrentHealth >= Target->MaxHealth)
	{
		return false;
	}
	*/

	Target->SetCurrentHealth(Target->CurrentHealth + HealAmount);

	return true;
}			

bool AHealthKit::CanPickupBy(AArcher* Character) const
{
	return Super::CanPickupBy(Character);
}
