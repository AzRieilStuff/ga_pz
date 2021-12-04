// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMovementComponent.h"

void UBaseMovementComponent::TickComponent(float DeltaTime, ELevelTick Tick,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	// calculate velocity
	Velocity = GetPendingInputVector().GetClampedToMaxSize(1.f) * DeltaTime * Speed;
	
	if (!GetPendingInputVector().IsNearlyZero())
	{
		UpdateComponentVelocity();
		
		FRotator DesiredRotation = GetWorld()->GetFirstPlayerController()->GetViewTarget()->GetActorRotation();
     
		// Interp to desired rotation
		//FRotator NewRotation = UpdatedComponent->GetComponentRotation();
		//NewRotation.Yaw = FMath::FixedTurn(UpdatedComponent->GetComponentRotation().Yaw, DesiredRotation.Yaw, 0.1f * DeltaTime);

		MoveUpdatedComponent(Velocity, UpdatedComponent->GetComponentRotation(), false);
	}

	ConsumeInputVector();
}
