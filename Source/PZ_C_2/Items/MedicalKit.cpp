// Fill out your copyright notice in the Description page of Project Settings.


#include "MedicalKit.h"
#include "PZ_C_2/Characters/BaseCharacter.h"

bool AMedicalKit::CanUseBy(ABaseCharacter* Character)
{
	return Character->Health < Character->MaxHealth;
}


void AMedicalKit::UseItem(ABaseCharacter* Character)
{
	Character->SetRegeneration(15.f, 3);
	Destroy(); 
}
