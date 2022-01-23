#include "HealthKitInventoryItem.h"
#include "AHealthKit.h"
#include "PZ_C_2/Characters/Archer.h"


UHealthKitInventoryItem::UHealthKitInventoryItem()
{
	VisualActorClass = AHealthKit::StaticClass();
}

bool UHealthKitInventoryItem::UseItem(AArcher* Target)
{
	Super::UseItem(Target);

	Target->SetCurrentHealth(Target->CurrentHealth + HealAmount);
	return true;
}

UBaseInventoryItem* AHealthKit::GenerateInventoryData(UBaseInventoryItem* Target) const
{
	UHealthKitInventoryItem* KitItem = NewObject<UHealthKitInventoryItem>();
	Super::GenerateInventoryData(KitItem);

	KitItem->HealAmount = HealAmount;

	return KitItem;
}


void UHealthKitInventoryItem::ServerUseHealthKit_Implementation(AArcher* Target, int32 RestoreAmount) const
{
	Target->SetCurrentHealth(Target->CurrentHealth + RestoreAmount);
}
