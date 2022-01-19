#pragma once

#include "PZ_C_2/Items/Core/BaseItem.h"
#include  "AHealthKit.generated.h"

/*
*
USTRUCT()
struct FHealthKitInventoryItem : public FInventoryItem
{
GENERATED_BODY()
	
};
*/

UCLASS()
class AHealthKit : public ABaseItem
{
	GENERATED_BODY()
protected:
public:
	AHealthKit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealAmount;

	virtual bool UseItem(AArcher* Target) override;

	virtual bool CanPickupBy(AArcher* Character) const override;
};
