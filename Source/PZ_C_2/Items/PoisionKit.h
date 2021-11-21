// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PoisionKit.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class PZ_C_2_API APoisionKit : public ABaseItem
{
	GENERATED_BODY()

public:
	virtual void UseItem(ABaseCharacter* Character) override;
};
