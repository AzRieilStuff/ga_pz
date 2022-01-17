// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IReloadable.h"
#include "PZ_C_2/Items/BaseItem.h"
#include "BaseRangeWeapon.generated.h"

class UWeaponManagerComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 InClip;

	UPROPERTY(BlueprintReadWrite)
	int32 Total;
};


UCLASS(Blueprintable)
class PZ_C_2_API ABaseRangeWeapon : public ABaseItem, public IReloadable
{
	GENERATED_BODY()

	// [local + server] run animation & timer
	void StartShootingTimer();

protected:
	// [server] 
	virtual class ABaseProjectile* SpawnProjectile(FVector AimLocation);
	
	// [local + server] runs for each client 
	virtual void OnShootingTimerEnd();
	
	// [server] handle shooting, spawn projectiles, shoot effect
	UFUNCTION(Server, Reliable)
	virtual void ServerPerformFire(FVector AimLocation);

	// [server] 
	virtual void ComputeProjectileTransform(const AArcher* Character, FVector AimLocation, FVector& Location,
	                                        FRotator& Rotation);

	// [local]
	FVector GetAimLocation(const AArcher* Character) const;

public:
	ABaseRangeWeapon();

	UPROPERTY()
	UWeaponManagerComponent* OwnerManagerComponent;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MaxAmmoTotal;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MaxAmmoInClip;

	UPROPERTY(BlueprintReadWrite)
	FAmmoData Ammo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Damage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ReloadDuration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Range;

	UFUNCTION(BlueprintCallable)
	virtual void FireAction();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerFireAction();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastFireAction();

	UPROPERTY(EditAnywhere, Category="Gameplay|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void RestoreAmmo();

	UFUNCTION(BlueprintCallable)
	bool CanFire() const;

	//~ Begin IReloadable Interface.
	virtual bool CanReload() const override;

	UFUNCTION(BlueprintCallable)
	virtual void Reload() override;
	//~ End IReloadable Interface

	UPROPERTY(BlueprintReadOnly)
	bool bIsReloading = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFiring = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireRate;

	UFUNCTION(Client, Reliable)
	void UseAmmo();

	FHitResult WeaponTrace(FVector& From, FVector& To);

	virtual bool CanPickupBy(AArcher* Character) const override;
};
