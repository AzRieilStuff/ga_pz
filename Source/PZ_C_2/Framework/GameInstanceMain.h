// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceMain.generated.h"

class USaveGameBase;

UCLASS()
class PZ_C_2_API UGameInstanceMain : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameInstanceMain();

	virtual void Init() override;
};
