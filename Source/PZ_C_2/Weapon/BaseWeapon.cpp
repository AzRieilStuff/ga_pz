// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "BaseProjectile.h"

ABaseWeapon::ABaseWeapon()
{
	Range = 10000;
	MuzzleSocketName = FName("Muzzle");

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Body");
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileClass = ABaseProjectile::StaticClass();

	bReplicates = true;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseWeapon::TryFire()
{
	if (!CanFire())
	{
		return;
	}

	USkeletalMeshSocket* Socket = MeshComponent->SkeletalMesh->FindSocket(MuzzleSocketName);
	if (Socket == nullptr)
	{
		return;
	}

	//FVector SocketLocation = Socket->GetSocketLocation(MeshComponent);
	//FVector TraceEndLocation = SocketLocation + (GetActorForwardVector() * Range);
	//WeaponTrace(SocketLocation, TraceEndLocation);

	Fire();
}

bool ABaseWeapon::Fire_Validate()
{
	return true;
}

void ABaseWeapon::Fire_Implementation()
{
	USkeletalMeshSocket* Socket = MeshComponent->SkeletalMesh->FindSocket(MuzzleSocketName);
	if (Socket == nullptr)
	{
		return;
	}

	FVector spawnLocation = Socket->GetSocketLocation(MeshComponent) + (Socket->GetSocketTransform(MeshComponent).
		Rotator().Vector() * 45.0f);
	FRotator spawnRotation = Socket->GetSocketTransform(MeshComponent).Rotator();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetParentActor()->GetInstigator();
	spawnParameters.Owner = this;

	GetWorld()->SpawnActor<ABaseProjectile>(
		spawnLocation, spawnRotation, spawnParameters);

	UseAmmo();
}

bool ABaseWeapon::CanReload() const
{
	return !bIsReloading && Ammo.InClip < MaxAmmoInClip && Ammo.Total > 0;
}

bool ABaseWeapon::CanFire() const
{
	return Ammo.InClip > 0 && !bIsReloading;
}

void ABaseWeapon::RestoreAmmo()
{
	int32 Restore = MaxAmmoInClip > Ammo.Total ? Ammo.Total : MaxAmmoInClip;
	Restore -= Ammo.InClip;
	Ammo.InClip += Restore;
	Ammo.Total -= Restore;
}

void ABaseWeapon::Reload()
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

void ABaseWeapon::UseAmmo_Implementation()
{
	Ammo.InClip--;
}

FHitResult ABaseWeapon::WeaponTrace(FVector& From, FVector& To)
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
