// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"

UBaseWeapon::UBaseWeapon()
{
	Range = 10000;
	MuzzleSocketName = FName("Muzzle");
}


void UBaseWeapon::Fire()
{
	if( !this->CanFire() ){
		return;
	}
	
	USkeletalMeshSocket* Socket = SkeletalMesh->FindSocket(this->MuzzleSocketName);
	if(Socket == nullptr){
		return;
	}
	
	FVector SocketLocation = Socket->GetSocketLocation(this);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, SocketLocation.ToString());

	FVector TraceEndLocation = SocketLocation + (GetOwner()->GetActorForwardVector() * Range);
	WeaponTrace(SocketLocation, TraceEndLocation);
	UseAmmo();
}

bool UBaseWeapon::bCanReload() const
{
	return !IsReloading && AmmoInClip < MaxAmmoInClip && AmmoTotal > 0; 
}

bool UBaseWeapon::CanFire() const
{
	return AmmoInClip > 0 && !IsReloading;
}

void UBaseWeapon::Reload()
{
	if( !bCanReload() || IsReloading ){
		return;
	}

	IsReloading = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloading"));

	FTimerHandle UnusedHandle;
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([&] 
	{
		int32 Restore = MaxAmmoInClip > AmmoTotal ? AmmoTotal : MaxAmmoInClip;
		Restore -= AmmoInClip;
		AmmoInClip += Restore;
		AmmoTotal -= Restore;
		IsReloading = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloaded"));
	});

	GetOwner()->GetWorldTimerManager().SetTimer(UnusedHandle, TimerCallback, ReloadDuration, false);
}

void UBaseWeapon::UseAmmo()
{
	AmmoInClip--;
}

void UBaseWeapon::WeaponTrace(FVector &From, FVector &To)
{
	FHitResult RV_Hit(ForceInit);
	
	const FName TraceTag("DebugTraceTag");
	FCollisionQueryParams CollisionTraceParams = FCollisionQueryParams(TraceTag, true, GetOwner());
	GetOwner()->GetWorld()->DebugDrawTraceTag = TraceTag;
	
	GetOwner()->GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		From,
		To,
		ECC_Pawn,
		CollisionTraceParams
	);
}