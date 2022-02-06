// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

#include "Slate/HealthBarWidget.h"

void UHealthBar::BeginDestroy()
{
	Super::BeginDestroy();
}

void UHealthBar::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
}

TSharedRef<SWidget> UHealthBar::RebuildWidget()
{
	return SNew(SHealthBarWidget).WorldContex(GetWorld());
}
