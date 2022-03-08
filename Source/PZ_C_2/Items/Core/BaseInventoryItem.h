#pragma once
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"

#include "BaseInventoryItem.generated.h"

class ABaseItem;
class AArcher;

UCLASS(Blueprintable, Abstract)
class UBaseInventoryItem : public UObject
{
	GENERATED_BODY()
protected:
public:
	UBaseInventoryItem();

	// static fails
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<ABaseItem> VisualActorClass;

	UPROPERTY(BlueprintReadOnly)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString IconLabel;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	UPROPERTY(BlueprintReadOnly)
	EInventorySlot SlotType;

	// [local]
	UFUNCTION(BlueprintCallable)
	virtual bool UseItem(AArcher* Target);

	UFUNCTION()
	virtual bool CanUsedBy(AArcher* Character) const;

	UFUNCTION()
	virtual bool CanUsedOn(AActor* Target) const;

	// helper method to access item static member from BPs	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABaseItem* GetItemDefaultObject() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual int32 GetStackLimit() const PURE_VIRTUAL(UBaseInventoryItem::GetStackLimit, return 1;);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool GetIsStackable() const { return GetStackLimit() > 1; };
};
