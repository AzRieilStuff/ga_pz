// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModeMain.h"

#include "PlayerControllerMain.h"
#include "PlayerStateMain.h"
#include "PZ_C_2/BaseCharacter.h"
#include "PlayerControllerMain.h"
#include "UObject/ConstructorHelpers.h"

AGameModeMain::AGameModeMain()
{
	PlayerStateClass = APlayerStateMain::StaticClass();
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = APlayerControllerMain::StaticClass();
}
