#include "PickableInterface.h"

bool IPickableInterface::CanPickupBy(AArcher* Character) const
{
	return false;
}

void IPickableInterface::TryPickup(AArcher* Character)
{
}
