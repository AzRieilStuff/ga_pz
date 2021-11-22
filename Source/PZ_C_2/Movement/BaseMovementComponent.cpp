// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMovementComponent.h"

#include "GameFramework/FloatingPawnMovement.h"

void UBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick Tick,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	// calculate velocity
	Velocity = GetPendingInputVector().GetClampedToMaxSize(1.f) * DeltaTime * Speed;
	
	if (!GetPendingInputVector().IsNearlyZero())
	{
		UpdatedComponent->ComponentVelocity = Velocity;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::White, Velocity.ToString());
	}

	ConsumeInputVector();
}
