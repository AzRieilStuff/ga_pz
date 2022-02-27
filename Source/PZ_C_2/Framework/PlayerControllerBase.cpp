// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "AbilitySystemComponent.h"
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

void APlayerControllerBase::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	AArcher* CharacterBase = Cast<AArcher>(P);
	if (CharacterBase)
	{
		CharacterBase->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterBase, CharacterBase);
	}
}

void APlayerControllerBase::ClientRestart_Implementation(APawn* NewPawn)
{
	Super::ClientRestart_Implementation(NewPawn);

}
