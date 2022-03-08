#include "RangeWeaponInventoryItem.h"

URangeWeaponInventoryItem::URangeWeaponInventoryItem()
{
	SlotType = EInventorySlot::MainWeapon;
}

int32 URangeWeaponInventoryItem::GetStackLimit() const
{
	return 1;
}
