// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModeMain.h"

#include "EngineUtils.h"
#include "PlayerControllerMain.h"
#include "PlayerStateMain.h"
#include "PZ_C_2/BaseCharacter.h"
#include "PZ_C_2/Items/BaseItem.h"
#include "PlayerControllerMain.h"
#include "UObject/ConstructorHelpers.h"

AGameModeMain::AGameModeMain()
{
	PlayerStateClass = APlayerStateMain::StaticClass();
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = APlayerControllerMain::StaticClass();
}

void AGameModeMain::BeginPlay()
{
	Super::BeginPlay();
	
	// clear all items from map on Player death. #todo delete
	ABaseCharacter* Character = Cast<ABaseCharacter, ACharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Character)
	{
		Character->OnDeathDelegate.BindUFunction(this, FName("ClearLevel"));
	}
	
}

void AGameModeMain::ClearLevel() const
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ClearLevel");
	
	for (TActorIterator<ABaseItem> It(GetWorld()); It; ++It)
	{
		(*It)->Destroy();
	}
}
