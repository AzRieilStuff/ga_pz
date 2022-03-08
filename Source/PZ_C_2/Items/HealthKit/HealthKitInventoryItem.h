#pragma once

#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "HealthKitInventoryItem.generated.h"

class AArcher;

UCLASS()
class UHealthKitInventoryItem : public UBaseInventoryItem
{
	GENERATED_BODY()
public :
	
	UFUNCTION(Server, Reliable)
	void ServerUseHealthKit(AArcher* Target, int32 RestoreAmount) const;

	int32 HealAmount;

	UHealthKitInventoryItem();

	virtual bool UseItem(AArcher* Target) override;

	virtual int32 GetStackLimit() const override;
};
