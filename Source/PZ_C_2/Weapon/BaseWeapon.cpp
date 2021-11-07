// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"

ABaseWeapon::ABaseWeapon()
{
	Range = 10000;
	MuzzleSocketName = FName("Muzzle");
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Body");
	MeshComponent->SetupAttachment(RootComponent);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseWeapon::Fire()
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

	FVector SocketLocation = Socket->GetSocketLocation(MeshComponent);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, SocketLocation.ToString());

	FVector TraceEndLocation = SocketLocation + (GetActorForwardVector() * Range);
	WeaponTrace(SocketLocation, TraceEndLocation);
	
	UseAmmo();
}

bool ABaseWeapon::CanReload() const
{
	return !bIsReloading && AmmoInClip < MaxAmmoInClip && AmmoTotal > 0;
}

bool ABaseWeapon::CanFire() const
{
	return AmmoInClip > 0 && !bIsReloading;
}

void ABaseWeapon::RestoreAmmo()
{
	int32 Restore = MaxAmmoInClip > AmmoTotal ? AmmoTotal : MaxAmmoInClip;
	Restore -= AmmoInClip;
	AmmoInClip += Restore;
	AmmoTotal -= Restore;
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

void ABaseWeapon::UseAmmo()
{
	AmmoInClip--;
}

void ABaseWeapon::WeaponTrace(FVector& From, FVector& To)
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
}
