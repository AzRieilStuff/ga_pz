// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameBase.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void USaveGameBase::PrepareSaveData(UWorld* World)
{
	for (TActorIterator<ATPCharacter> ActorItr(World); ActorItr; ++ActorItr)
	{
		const ATPCharacter* Character = *ActorItr;

		if (IsValid(Character))
		{
			CharactersData.Emplace(Character->GetName(), Character->GetSaveData());
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Save %s"), *Character->GetName()));

		}
	}
}

void USaveGameBase::ProcessSaveData(UWorld* World)
{
	for (TActorIterator<ATPCharacter> ActorItr(World); ActorItr; ++ActorItr)
	{
		ATPCharacter* Character = *ActorItr;

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
