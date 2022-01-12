// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameBase.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void USaveGameBase::PrepareSaveData(UWorld* World)
{
	for (TActorIterator<AArcher> ActorItr(World); ActorItr; ++ActorItr)
	{
		const AArcher* Character = *ActorItr;

		if (IsValid(Character))
		{
			CharactersData.Emplace(Character->GetName(), Character->GetSaveData());
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Save %s"), *Character->GetName()));

		}
	}
}

void USaveGameBase::ProcessSaveData(UWorld* World)
{
	for (TActorIterator<AArcher> ActorItr(World); ActorItr; ++ActorItr)
	{
		AArcher* Character = *ActorItr;

		if (IsValid(Character) && CharactersData.Contains(Character->GetName()))
		{
			Character->InitFromSaveData(CharactersData[Character->GetName()]);
		}
	}
}

USaveGameBase::USaveGameBase()
{
	SaveSlotName = TEXT("SaveGame");
	UserIndex = 0;
}
