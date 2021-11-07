// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API APlayerCharacter : public ABaseCharacter, public IContainer
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	float Stamina;

public:
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UInventory* InventoryComponent;
	
	APlayerCharacter();

	// IContainer
	UFUNCTION(BlueprintCallable)
	virtual UInventory* GetInventory() override;

	virtual void InitInventory() override;
	//
};
