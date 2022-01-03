// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeMain.generated.h"

UCLASS(minimalapi)
class AGameModeMain : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeMain();

	virtual void BeginPlay() override;
};
