// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
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
	virtual void InitInventory() = 0;
	virtual UInventory* GetInventory() = 0;
};
