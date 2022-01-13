// Fill out your copyright notice in the Description page of Project Settings.


#include "ShortgunBow.h"

void AShortgunBow::PerformFire()
{
	// spawn multiple arrows
	TArray<ABaseProjectile*> Spawned;
	for (int32 i = 0; i < ProjectilesAmount; i++)
	{
		if (ABaseProjectile* Arrow = SpawnProjectile())
		{
			Spawned.Add(Arrow);
			Arrow->MovementComponent->StopMovementImmediately();
		}
	}

	// slightly rotate
	for (int32 i = 0; i < Spawned.Num(); i++)
	{
		float Amplitude = 10.f;

		FVector ArrowRotation = FVector(
			FMath::RandRange(-Amplitude, Amplitude),
			FMath::RandRange(-Amplitude, Amplitude),
			FMath::RandRange(-Amplitude, Amplitude)
		);
		FRotator Rotation = Spawned[i]->GetActorRotation();
		Rotation.Pitch += FMath::RandRange(-Amplitude, Amplitude);
		Rotation.Yaw += FMath::RandRange(-Amplitude, Amplitude);
		Spawned[i]->SetActorRotation(Rotation);
		
		Spawned[i]->EnableMovement();

	}
}

AShortgunBow::AShortgunBow()
{
	ProjectilesAmount = 3;
}
