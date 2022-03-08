#pragma once
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "RangeWeaponInventoryItem.generated.h"

UCLASS()
class URangeWeaponInventoryItem : public UBaseInventoryItem
{
	GENERATED_BODY()
public:
	URangeWeaponInventoryItem();
	
	virtual int32 GetStackLimit() const override;
};
