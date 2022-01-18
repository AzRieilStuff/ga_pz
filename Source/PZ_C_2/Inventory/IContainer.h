// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryManagerComponent.h"
#include "IContainer.generated.h"

UINTERFACE()
class PZ_C_2_API UContainer : public UInterface
{
	GENERATED_BODY()
public:
};

class PZ_C_2_API IContainer
{
	GENERATED_BODY()
public:
	virtual UInventoryManagerComponent* GetInventory() const = 0;
};
