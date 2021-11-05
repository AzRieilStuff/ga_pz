// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IReloadable.generated.h"

UINTERFACE()
class PZ_C_2_API UReloadable : public UInterface
{
	GENERATED_BODY() 
};

class PZ_C_2_API IReloadable
{
	GENERATED_BODY() 
public:
	virtual bool bCanReload() const = 0;
	virtual void Reload() = 0;
};
