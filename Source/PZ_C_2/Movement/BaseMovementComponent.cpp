// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMovementComponent.h"

void UBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick Tick,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	if (!GetPendingInputVector().IsZero())
	{
		FVector NewLocation = GetActorLocation() + (ConsumeInputVector() * DeltaTime * Speed);
		PawnOwner->SetActorLocation(NewLocation);
	}
}
