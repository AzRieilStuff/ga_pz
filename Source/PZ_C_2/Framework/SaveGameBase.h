// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PZ_C_2/Characters/Archer.h"
#include "SaveGameBase.generated.h"

/**
 * 
 */
UCLASS()
class PZ_C_2_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TMap<FString, FCharacterSaveData> CharactersData;

	void PrepareSaveData(UWorld* World);

	void ProcessSaveData(UWorld* World);

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	USaveGameBase();
};
