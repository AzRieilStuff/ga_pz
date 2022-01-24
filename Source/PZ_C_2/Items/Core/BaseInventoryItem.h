#pragma once

#include "BaseInventoryItem.generated.h"

class ABaseItem;
class AArcher;

UCLASS(Blueprintable)
class UBaseInventoryItem : public UObject
{
	GENERATED_BODY()

	/**
	 * @brief Is currently processing
	 */
	bool bLocked = false;

	//friend class UInventoryManagerComponent;

protected:
public:
	inline bool IsLocked() const { return bLocked; };

	inline void Lock() { bLocked = true; }

	inline void Unlock() { bLocked = false; }

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

};
