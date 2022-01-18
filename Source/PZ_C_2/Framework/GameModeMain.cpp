// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModeMain.h"

#include "EngineUtils.h"
#include "PlayerControllerMain.h"
#include "PlayerStateMain.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include "PlayerControllerMain.h"
#include "UObject/ConstructorHelpers.h"

AGameModeMain::AGameModeMain()
{
	PlayerStateClass = APlayerStateMain::StaticClass();
	DefaultPawnClass = AArcher::StaticClass();
	PlayerControllerClass = APlayerControllerMain::StaticClass();
}

void AGameModeMain::BeginPlay()
{
	Super::BeginPlay();
	
	
}
