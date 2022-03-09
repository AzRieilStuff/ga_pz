// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseRangeWeapon.h"

#include "CollisionQueryParams.h"
#include "RangeWeaponInventoryItem.h"
#include "TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PZ_C_2/Ammo/BaseProjectile.h"
#include "PZ_C_2/Characters/Archer.h"


ABaseRangeWeapon::ABaseRangeWeapon()
{
	ProjectileClass = ABaseProjectile::StaticClass();

	bDestroyOnPickup = false;

	// should be replicated with weapon owner
	bNetUseOwnerRelevancy = true;

	// update ammo info
	NetUpdateFrequency = 3.f;

	MaxPerStack = 1;
}


void ABaseRangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABaseRangeWeapon, OwnerManagerComponent, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ABaseRangeWeapon, Ammo, COND_OwnerOnly);
}

void ABaseRangeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

UBaseInventoryItem* ABaseRangeWeapon::GenerateInventoryData(UBaseInventoryItem* Target) const
{
	URangeWeaponInventoryItem* Item = NewObject<URangeWeaponInventoryItem>();
	Super::GenerateInventoryData(Item);

	return Item;
}


void ABaseRangeWeapon::ServerPerformFire_Implementation(FVector AimLocation)
{
	ABaseProjectile* Arrow = SpawnProjectile(AimLocation, true);
	if (Arrow == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to replicate shooting on arrow spawn"));
		return;
	}

	// Do not replicate arrow as one was shoot already from client
	Arrow->SetReplicates(false);
	Arrow->FinishSpawning(Arrow->GetTransform());

	Arrow->OnShoot();

	// keep inventory state synchronized with server
	OwnerManagerComponent->Character->GetInventoryManagerComponent()->ConsumeItem(
		EInventorySlot::MainWeaponAmmo, 1
	);

	MulticastPerformFire(AimLocation);
}

void ABaseRangeWeapon::MulticastPerformFire_Implementation(const FVector AimLocation)
{
	// Commit shoot visuals for all but server & client who perform it
	if (GetOwner()->GetLocalRole() != ROLE_SimulatedProxy || GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		return;
	}

	ABaseProjectile* Arrow = SpawnProjectile(AimLocation);
	Arrow->OnShoot();
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

	// todo improve ai detection?
	if (GetInstigatorController() && GetInstigatorController()->IsPlayerController()) // player
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
	//Params.AddIgnoredActor(Character);
	//GetWorld()->DebugDrawTraceTag = TraceTag;

	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params);
	FVector AimTarget = (bHit ? Hit.Location : TraceEnd);

	return AimTarget;
}

ABaseProjectile* ABaseRangeWeapon::SpawnProjectile(FVector AimLocation, const bool DeferredSpawn)
{
	return nullptr;
}

void ABaseRangeWeapon::Fire()
{
	// from server or client but only once per shoot
	FVector Aim = GetAimLocation(OwnerManagerComponent->Character);

	ABaseProjectile* Arrow = SpawnProjectile(Aim);
	Arrow->OnShoot();

	OwnerManagerComponent->Character->GetInventoryManagerComponent()->ConsumeItem(
		EInventorySlot::MainWeaponAmmo, 1
	);

	if (GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		ServerPerformFire(Aim);
	}
	else
	{
		MulticastPerformFire(Aim);
	}
}

bool ABaseRangeWeapon::ServerPerformFire_Validate(FVector AimLocation)
{
	return CanFire(); // kick bastard if he is firing without ammo
}

bool ABaseRangeWeapon::CanReload() const
{
	return !bIsReloading && Ammo.InClip < MaxAmmoInClip && Ammo.Total > 0;
}

bool ABaseRangeWeapon::CanFire() const
{
	return OwnerManagerComponent->HasAmmo();
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloading"));

	FTimerHandle UnusedHandle;
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([&]
	{
		RestoreAmmo();
		bIsReloading = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloaded"));
	});

	GetWorldTimerManager().SetTimer(UnusedHandle, TimerCallback, ReloadDuration, false);
}

void ABaseRangeWeapon::UseAmmo_Implementation()
{
	Ammo.InClip--;
}

bool ABaseRangeWeapon::CanPickupBy(AArcher* Character) const
{
	return bPickable && Character->GetWeaponManagerComponent()->CanEquipWeapon(this);
}

void ABaseRangeWeapon::ServerPickup(AArcher* Character)
{
	//Character->WeaponManagerComponent->CurrentWeapon = this;
	Character->GetWeaponManagerComponent()->EquipWeapon(this);

	Super::ServerPickup(Character);
}
