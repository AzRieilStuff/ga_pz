// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisionKit.h"

void APoisionKit::UseItem(ABaseCharacter* Character)
{
	Character->SetRegeneration(-25.f, 3);
	Destroy();
}
