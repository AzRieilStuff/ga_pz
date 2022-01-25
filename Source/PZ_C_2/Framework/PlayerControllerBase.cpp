// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "GameInstanceMain.h"
#include "PZ_C_2/Characters/Archer.h"

APlayerControllerBase::APlayerControllerBase()
{
	SetShowMouseCursor(true);
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
