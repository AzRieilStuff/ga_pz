// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"

AArcher* UHelpers::GetArcherCharacter(const UWorld* WorldContext)
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(WorldContext, 0);
	if (Character != nullptr)
	{
		return Cast<AArcher>(Character);
	}
	
	return nullptr;
}
