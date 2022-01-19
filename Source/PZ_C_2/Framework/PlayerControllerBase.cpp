// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "GameInstanceMain.h"

APlayerControllerBase::APlayerControllerBase()
{
	SetShowMouseCursor(true);
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}
