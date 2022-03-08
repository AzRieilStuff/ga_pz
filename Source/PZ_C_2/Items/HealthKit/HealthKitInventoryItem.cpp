#include "HealthKitInventoryItem.h"
#include "AHealthKit.h"
#include "PZ_C_2/Characters/Archer.h"

UHealthKitInventoryItem::UHealthKitInventoryItem()
{
	VisualActorClass = AHealthKit::StaticClass();
	SlotType = EInventorySlot::Consumable;
}

bool UHealthKitInventoryItem::UseItem(AArcher* Target)
{
	Super::UseItem(Target);

	//Target->SetCurrentHealth(Target->CurrentHealth + HealAmount);
	return true;
}

int32 UHealthKitInventoryItem::GetStackLimit() const
{
	return 5;
}


void UHealthKitInventoryItem::ServerUseHealthKit_Implementation(AArcher* Target, int32 RestoreAmount) const
{
	//Target->SetCurrentHealth(Target->CurrentHealth + RestoreAmount);
}
