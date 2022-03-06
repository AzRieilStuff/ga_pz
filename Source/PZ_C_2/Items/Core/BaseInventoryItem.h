#pragma once
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"

#include "BaseInventoryItem.generated.h"

class ABaseItem;
class AArcher;

UCLASS(Blueprintable)
class UBaseInventoryItem : public UObject
{
	GENERATED_BODY()
protected:
public:
	UBaseInventoryItem();

	// static fails
	TSubclassOf<ABaseItem> VisualActorClass;

	UPROPERTY(BlueprintReadOnly)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString IconLabel;

	// [local]
	UFUNCTION(BlueprintCallable)
	virtual bool UseItem(AArcher* Target);

	UFUNCTION()
	virtual bool CanUsedBy(AArcher* Character) const;

	UFUNCTION()
	virtual bool CanUsedOn(AActor* Target) const;
	
	UPROPERTY()
	int32 Amount;

	EInventorySlot SlotType;
};
