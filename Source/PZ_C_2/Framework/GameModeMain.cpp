// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModeMain.h"

#include "PlayerStateMain.h"
#include "UObject/ConstructorHelpers.h"

AGameModeMain::AGameModeMain()
{
	this->PlayerStateClass = APlayerStateMain::StaticClass();
}

bool AGameModeMain::IsAndroid() const
{
#if PLATFORM_ANDROID
	return true;
#endif
	return false;
}