// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceMain.generated.h"

class USaveGameBase;

UCLASS()
class PZ_C_2_API UGameInstanceMain : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameInstanceMain();

	UPROPERTY()
	USaveGameBase* SaveGame;

	UFUNCTION()
	void QuickSave();

	UFUNCTION(Server, Reliable)
	void QuickSaveServer();

	UFUNCTION()
	void QuickLoad();
	
	UFUNCTION(Server, Reliable)
	void QuickLoadServer();

	virtual void Init() override;
};
