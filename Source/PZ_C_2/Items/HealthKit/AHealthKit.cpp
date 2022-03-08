#include "AHealthKit.h"

#include "HealthKitInventoryItem.h"
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

UBaseInventoryItem* AHealthKit::GenerateInventoryData(UBaseInventoryItem* Target) const
{
	UHealthKitInventoryItem* KitItem = NewObject<UHealthKitInventoryItem>();
	Super::GenerateInventoryData(KitItem);

	KitItem->HealAmount = HealAmount;

	return KitItem;
}
