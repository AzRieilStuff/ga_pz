// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceMain.h"

#include "SaveGameBase.h"
#include "Kismet/GameplayStatics.h"

UGameInstanceMain::UGameInstanceMain()
{
}

void UGameInstanceMain::QuickSave()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Try save"));
	QuickSaveServer();
}

void UGameInstanceMain::QuickSaveServer_Implementation()
{
	if (USaveGameBase* SaveGameInstance = Cast<USaveGameBase>(
		UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass())))
	{
		SaveGameInstance->PrepareSaveData(GetWorld());

		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "SaveGame", 0))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Saved"));
		}
	}
}

void UGameInstanceMain::QuickLoad()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Try load"));

	// allow load from server only
	if (GetFirstLocalPlayerController()->GetPawn()->HasAuthority())
	{
		QuickLoadServer();
	}
}

void UGameInstanceMain::QuickLoadServer_Implementation()
{
	if (USaveGameBase* LoadedGame = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot("SaveGame", 0)))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Loaded"));
		LoadedGame->ProcessSaveData(GetWorld());
	}
}


void UGameInstanceMain::Init()
{
	Super::Init();
}
