// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IReloadable.h"
#include "PZ_C_2/Items/BaseItem.h"
#include "BaseRangeWeapon.generated.h"

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

	void SetFireRateTimer();


	virtual class ABaseProjectile* SpawnProjectile();
protected:

	// [server]
	virtual void PerformFire();

	virtual void ComputeProjectileTransform(const AArcher* Character, FVector& Location, FRotator& Rotation);
public:
	ABaseRangeWeapon();

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
	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void FireServer();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void FireMulticast();

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

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float FireRate;

	UFUNCTION(Client, Reliable)
	void UseAmmo();

	FHitResult WeaponTrace(FVector& From, FVector& To);

	
};
