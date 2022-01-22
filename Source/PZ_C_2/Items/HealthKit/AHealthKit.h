#pragma once

#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include  "AHealthKit.generated.h"

class UBaseInventoryItem;

UCLASS()
class UHealthKitInventoryItem : public UBaseInventoryItem
{
	GENERATED_BODY()
public :
	virtual bool UseItem(AArcher* Target) override;

	UFUNCTION(Server, Reliable)
	void ServerUseHealthKit(AArcher* Target, int32 RestoreAmount) const;
	
	int32 HealAmount;

	UHealthKitInventoryItem();
};

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

	virtual UHealthKitInventoryItem* GenerateInventoryData(UBaseInventoryItem* Target = nullptr) const override;
};
