#pragma once

#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include  "AHealthKit.generated.h"

class UBaseInventoryItem;
class UHealthKitInventoryItem;

UCLASS()
class AHealthKit : public ABaseItem
{
	GENERATED_BODY()
protected:
public:
	AHealthKit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealAmount;

	virtual bool CanPickupBy(AArcher* Character) const override;

	virtual UBaseInventoryItem* GenerateInventoryData(UBaseInventoryItem* Target = nullptr) const override;
};
