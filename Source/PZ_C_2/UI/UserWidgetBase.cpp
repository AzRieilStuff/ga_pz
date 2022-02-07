// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetBase.h"

void UUserWidgetBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void UUserWidgetBase::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
}

bool UUserWidgetBase::Initialize()
{
	return Super::Initialize();
}
