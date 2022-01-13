#include "PickableInterface.h"

bool IPickableInterface::CanPickupBy(AArcher* Character) const
{
	return false;
}

void IPickableInterface::Pickup(AArcher* Character)
{
}
