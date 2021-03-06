// Fill out your copyright notice in the Description page of Project Settings.


#include "ShortgunBow.h"

void AShortgunBow::ServerPerformFire_Implementation(FVector AimLocation)
{
	// spawn multiple arrows
	//TArray<ABaseProjectile*> Spawned;
	for (int32 i = 0; i < ProjectilesAmount; i++)
	{
		if (ABaseProjectile* Arrow = SpawnProjectile(AimLocation))
		{
			//Arrow->EnableMovement();
		}
	}
}

AShortgunBow::AShortgunBow()
{
	ProjectilesAmount = 3;
}

void AShortgunBow::ComputeProjectileTransform(const AArcher* Character, FVector AimLocation, FVector& Location,
                                              FRotator& Rotation)
{
	Super::ComputeProjectileTransform(Character, AimLocation, Location, Rotation);

	// add rotation	
	const float Amplitude = 10.f;
	Rotation.Pitch += FMath::RandRange(-Amplitude, Amplitude);
	Rotation.Yaw += FMath::RandRange(-Amplitude, Amplitude);
}
