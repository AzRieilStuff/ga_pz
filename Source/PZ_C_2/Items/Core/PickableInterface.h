#pragma once

#include "PickableInterface.generated.h"

class AArcher;

UINTERFACE()
class UPickableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPickableInterface
{
	GENERATED_BODY()

	virtual bool CanPickupBy(AArcher* Character) const;

	virtual void TryPickup(AArcher* Character);
};