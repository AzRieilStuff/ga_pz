// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"

UBaseWeapon::UBaseWeapon()
{
}


void UBaseWeapon::Fire()
{
	if( !this->CanFire() )
		return;
	
	auto* Socket = SkeletalMesh->FindSocket(this->MuzzleSocketName);
	if(Socket == nullptr)
		return;
	
	FVector SocketLocation = Socket->GetSocketLocation(this);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, SocketLocation.ToString());
	this->WeaponTrace(SocketLocation, SocketLocation + (GetOwner()->GetActorForwardVector() * 10000));
	this->UseAmmo();
}

bool UBaseWeapon::CanReload() const
{
	return this->AmmoInClip < this->MaxAmmoInClip && this->AmmoTotal > 0; 
}

bool UBaseWeapon::CanFire() const
{
	return this->AmmoInClip > 0 && !this->IsReloading;
}

void UBaseWeapon::Reload()
{
	if( !this->CanReload() || this->IsReloading )
		return;

	this->IsReloading = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloading"));

	FTimerHandle UnusedHandle;
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([&] 
	{
		int32 Restore = this->MaxAmmoInClip > this->AmmoTotal ? this->AmmoTotal : this->MaxAmmoInClip;
		Restore -= this->AmmoInClip;
		this->AmmoInClip += Restore;
		this->AmmoTotal -= Restore;
		this->IsReloading = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Reloaded"));
	});

	GetOwner()->GetWorldTimerManager().SetTimer(UnusedHandle, TimerCallback, this->ReloadDuration, false);
}

void UBaseWeapon::UseAmmo()
{
	this->AmmoInClip--;
}

void UBaseWeapon::WeaponTrace(FVector from, FVector to)
{
	FHitResult RV_Hit(ForceInit);
	
	const FName TraceTag("DebugTraceTag");
	FCollisionQueryParams CollisionTraceParams = FCollisionQueryParams(TraceTag, true, GetOwner());
	GetOwner()->GetWorld()->DebugDrawTraceTag = TraceTag;
	
	GetOwner()->GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		from,
		to,
		ECC_Pawn,
		CollisionTraceParams
	);
}