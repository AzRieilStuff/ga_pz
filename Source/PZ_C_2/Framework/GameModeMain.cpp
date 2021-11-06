// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModeMain.h"

#include "PlayerControllerMain.h"
#include "PlayerStateMain.h"
#include "PZ_C_2/CharacterMain.h"
#include "PlayerControllerMain.h"
#include "UObject/ConstructorHelpers.h"

AGameModeMain::AGameModeMain()
{
	PlayerStateClass = APlayerStateMain::StaticClass();
	DefaultPawnClass = ACharacterMain::StaticClass();
	PlayerControllerClass = APlayerControllerMain::StaticClass();
}

bool AGameModeMain::IsAndroid() const
{
#if PLATFORM_ANDROID
	return true;
#endif
	return false;
}
