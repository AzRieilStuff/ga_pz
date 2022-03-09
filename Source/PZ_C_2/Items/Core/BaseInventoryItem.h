#pragma once
#include "BaseInventoryItem.generated.h"

class ABaseItem;
class AArcher;
class UInventoryManagerComponent;

enum class EInventorySlot : uint8;

UCLASS(Blueprintable, Abstract)
class UBaseInventoryItem : public UObject
{
	GENERATED_BODY()

	int32 Amount;

public:
	UBaseInventoryItem();

	UBaseInventoryItem(EInventorySlot Slot, TSubclassOf<ABaseItem> ActorClass, int32 InitAmount, FString ItemName,
	                   UTexture* IconTexture);

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<ABaseItem> VisualActorClass;

	UPROPERTY(BlueprintReadOnly)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString IconLabel;

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool GetIsStackFull() const { return Amount == GetStackLimit(); };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline int32 GetAmount() const { return Amount; };

	void SetAmount(const int NewAmount);

	void ModifyAmount(const int ModValue);
	
};
