// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MedicalKit.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class PZ_C_2_API AMedicalKit : public ABaseItem
{
	GENERATED_BODY()
	virtual bool CanUseBy(ABaseCharacter* Character) override;
	virtual void UseItem(ABaseCharacter* Character) override;
};
