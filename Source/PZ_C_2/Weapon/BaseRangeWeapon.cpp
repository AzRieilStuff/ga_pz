// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRangeWeapon.h"

#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PZ_C_2/Ammo/BaseProjectile.h"
#include "PZ_C_2/Characters/Archer.h"

void ABaseRangeWeapon::StartShootingTimer()
{
	bIsFiring = true;

	FTimerHandle FiringTimerHandle; // todo move to class, shoot cancellation
	GetWorldTimerManager().SetTimer(FiringTimerHandle, this, &ABaseRangeWeapon::OnShootingTimerEnd, FireRate, false);
}

void ABaseRangeWeapon::ServerPerformFire_Implementation(FVector AimLocation)
{
	ABaseProjectile* Projectile = SpawnProjectile(AimLocation);
}

void ABaseRangeWeapon::ComputeProjectileTransform(const AArcher* Character, FVector AimLocation, FVector& Location,
                                                  FRotator& Rotation)
{
	AimLocation -= Location;
	AimLocation.Normalize();
	Rotation = AimLocation.Rotation();
}

FVector ABaseRangeWeapon::GetAimLocation(const AArcher* Character) const
{
	// projectile start position/rotation ( from socket ) can differ from screen center, so need to be adjusted
	FVector TraceStart, TraceEnd;

	if( GetInstigatorController() && GetInstigatorController()->IsPlayerController() ) // player
	{
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		TraceStart = CameraManager->GetCameraLocation();
		TraceEnd = TraceStart + (CameraManager->GetActorForwardVector() * 3000);
	}
	else // AI
	{
		TraceStart = GetOwner()->GetActorLocation();
		TraceEnd = TraceStart + (GetOwner()->GetActorForwardVector() * 3000);
	}

	FHitResult Hit;
	FCollisionQueryParams Params;

	const FName TraceTag("Debug");

	Params.TraceTag = TraceTag;
	Params.AddIgnoredActor(Character);
	//GetWorld()->DebugDrawTraceTag = TraceTag;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params);
	FVector AimTarget = (bHit ? Hit.Location : TraceEnd);

	return AimTarget;
}

ABaseProjectile* ABaseRangeWeapon::SpawnProjectile(FVector AimLocation)
{
	return nullptr;
}

void ABaseRangeWeapon::OnShootingTimerEnd()
{
	bIsFiring = false;

	FVector Aim = GetAimLocation(OwnerManagerComponent->Character);

	// from server or client but only once per shoot
	if (OwnerManagerComponent->Character->IsLocallyControlled())
	{
		ServerPerformFire(Aim);
	}
}

ABaseRangeWeapon::ABaseRangeWeapon()
{
	ProjectileClass = ABaseProjectile::StaticClass();

	bDestroyOnPickup = false;
	bStoreable = false;

	// should be replicated with weapon owner
	bNetUseOwnerRelevancy = true;
}

void ABaseRangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABaseRangeWeapon, OwnerManagerComponent, COND_InitialOnly);
}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseRangeWeapon::FireAction()
{
	if (bIsFiring)
	{
		return;
	}

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		StartShootingTimer();
	}

	ServerFireAction();
}

void ABaseRangeWeapon::ServerFireAction_Implementation()
{
	StartShootingTimer();
	MulticastFireAction();
}

void ABaseRangeWeapon::MulticastFireAction_Implementation()
{
	// run visuals for simulated within clients
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		StartShootingTimer();
	}
}

bool ABaseRangeWeapon::ServerFireAction_Validate()
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

bool ABaseRangeWeapon::CanPickupBy(AArcher* Character) const
{
	return bPickable && Character->WeaponManagerComponent->CanEquipWeapon(this);
}

void ABaseRangeWeapon::ServerPickup(AArcher* Character)
{
	Character->WeaponManagerComponent->CurrentWeapon = this;
	
	Super::ServerPickup(Character);
}
