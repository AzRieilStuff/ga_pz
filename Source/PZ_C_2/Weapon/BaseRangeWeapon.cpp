// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRangeWeapon.h"

#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PZ_C_2/Ammo/BaseProjectile.h"

void ABaseRangeWeapon::SetFireRateTimer()
{
	bIsFiring = true;

	FTimerHandle FiringTimerHandle;
	GetWorldTimerManager().SetTimer(FiringTimerHandle, [this]
	{
		bIsFiring = false;
		if (GetOwner()->GetLocalRole() == ROLE_Authority)
		{
			PerformFire();
		}
	}, FireRate, false);
}

void ABaseRangeWeapon::PerformFire()
{
	ABaseProjectile* Projectile = SpawnProjectile();
	if( Projectile != nullptr )
	{
		//Projectile->EnableMovementMulticast();
	}
}

void ABaseRangeWeapon::ComputeProjectileTransform(const AArcher* Character, FVector& Location, FRotator& Rotation)
{
}


ABaseProjectile* ABaseRangeWeapon::SpawnProjectile()
{
	return nullptr;
}

ABaseRangeWeapon::ABaseRangeWeapon()
{
	Range = 10000;
	ProjectileClass = ABaseProjectile::StaticClass();

	bDestroyOnPickup = false;
	FireRate = 0.5f;
}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseRangeWeapon::Fire()
{
	if (bIsFiring)
	{
		return;
	}

	// If calling from client, allow to execute instantly
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		SetFireRateTimer();
	}

	FireServer();
}

void ABaseRangeWeapon::FireServer_Implementation()
{
	SetFireRateTimer();
	FireMulticast();
}

void ABaseRangeWeapon::FireMulticast_Implementation()
{
	// run visuals for simulated within clients
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		SetFireRateTimer();
	}
}

bool ABaseRangeWeapon::FireServer_Validate()
{
	if (bIsFiring)
	{
		return false;
	}

	return Ammo.InClip > 0;
}

bool ABaseRangeWeapon::CanReload() const
{
	return !bIsReloading && Ammo.InClip < MaxAmmoInClip && Ammo.Total > 0;
}

bool ABaseRangeWeapon::CanFire() const
{
	return Ammo.InClip > 0 && !bIsReloading;
}

void ABaseRangeWeapon::RestoreAmmo()
{
	int32 Restore = MaxAmmoInClip > Ammo.Total ? Ammo.Total : MaxAmmoInClip;
	Restore -= Ammo.InClip;
	Ammo.InClip += Restore;
	Ammo.Total -= Restore;
}

void ABaseRangeWeapon::Reload()
{
	if (!CanReload() || bIsReloading)
	{
		return;
	}

	bIsReloading = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloading"));

	FTimerHandle UnusedHandle;
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([&]
	{
		RestoreAmmo();
		bIsReloading = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloaded"));
	});

	GetWorldTimerManager().SetTimer(UnusedHandle, TimerCallback, ReloadDuration, false);
}

void ABaseRangeWeapon::UseAmmo_Implementation()
{
	Ammo.InClip--;
}

FHitResult ABaseRangeWeapon::WeaponTrace(FVector& From, FVector& To)
{
	FHitResult RV_Hit(ForceInit);

	const FName TraceTag("DebugTraceTag");
	FCollisionQueryParams CollisionTraceParams = FCollisionQueryParams(TraceTag, true, this);
	GetWorld()->DebugDrawTraceTag = TraceTag;

	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		From,
		To,
		ECC_Pawn,
		CollisionTraceParams
	);

	return RV_Hit;
}
