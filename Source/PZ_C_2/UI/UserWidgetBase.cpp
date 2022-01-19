// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetBase.h"

void UUserWidgetBase::BeginDestroy()
{
	Super::BeginDestroy();
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "destroy");
}

void UUserWidgetBase::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "visibility");
}

bool UUserWidgetBase::Initialize()
{
	return Super::Initialize();
}
