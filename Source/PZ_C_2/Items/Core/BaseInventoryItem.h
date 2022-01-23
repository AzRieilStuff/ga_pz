#pragma once

#include "BaseInventoryItem.generated.h"

class ABaseItem;
class AArcher;

UCLASS(Blueprintable)
class UBaseInventoryItem : public UObject
{
	GENERATED_BODY()

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

	UPROPERTY()
	int32 Amount;

	UFUNCTION(BlueprintCallable)
	virtual bool UseItem(AArcher* Target);

	UFUNCTION()
	virtual bool CanUsedBy(AArcher* Character) const;

	UFUNCTION()
	virtual bool CanUsedOn(AActor* Target) const;
};
