#include "AHealthKit.h"

#include "PZ_C_2/Characters/Archer.h"


UHealthKitInventoryItem::UHealthKitInventoryItem()
{
	VisualActorClass = AHealthKit::StaticClass();
}

AHealthKit::AHealthKit()
{
	HealAmount = 10;
	bDestroyOnPickup = true;
	bStoreable = true;
	bPickable = true;
}

bool AHealthKit::CanPickupBy(AArcher* Character) const
{
	return Super::CanPickupBy(Character);
}

UHealthKitInventoryItem* AHealthKit::GenerateInventoryData(UBaseInventoryItem* Target) const
{
	UHealthKitInventoryItem* KitItem = NewObject<UHealthKitInventoryItem>();
	Super::GenerateInventoryData(KitItem);

	KitItem->HealAmount = HealAmount;

	return KitItem;
}


bool UHealthKitInventoryItem::UseItem(AArcher* Target)
{
	Super::UseItem(Target);

	if (HealAmount >= 0 && Target->CurrentHealth >= Target->MaxHealth)
	{
		return false;
	}

	ServerUseHealthKit(Target, HealAmount);

	return true;
}

void UHealthKitInventoryItem::ServerUseHealthKit_Implementation(AArcher* Target, int32 RestoreAmount) const
{
	Target->SetCurrentHealth(Target->CurrentHealth + RestoreAmount);
}	
