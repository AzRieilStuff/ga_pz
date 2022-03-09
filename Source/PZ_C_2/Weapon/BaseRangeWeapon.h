// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IReloadable.h"
#include "PZ_C_2/Items/Core/BaseItem.h"
#include "BaseRangeWeapon.generated.h"

class UWeaponManagerComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Total;
};


UCLASS(Blueprintable, Config=Weapon, defaultconfig)
class PZ_C_2_API ABaseRangeWeapon : public ABaseItem, public IReloadable
{
private:
	GENERATED_BODY()

	friend class UWeaponManagerComponent;
public:
	ABaseRangeWeapon();

	UPROPERTY(Replicated)
	UWeaponManagerComponent* OwnerManagerComponent;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual UBaseInventoryItem* GenerateInventoryData(UBaseInventoryItem* Target) const override;

#pragma region Firing
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Config, Category=WeaponStats)
	float AimingDuration;
	
protected:
	// [server] 
	virtual class ABaseProjectile* SpawnProjectile(FVector AimLocation, const bool DeferredSpawn = false);

	// [local + server] runs for each client, invokes from ability
	virtual void Fire();

	// [server] handle shooting, spawn projectiles, shoot effect
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerPerformFire(FVector AimLocation);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPerformFire(const FVector AimLocation);

	// [server] 
	virtual void ComputeProjectileTransform(const AArcher* Character, FVector AimLocation, FVector& Location,
	                                        FRotator& Rotation);

	// [local] get endpoint of trace from camera view
	FVector GetAimLocation(const AArcher* Character) const;

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="WeaponStats")
	int32 MaxAmmoTotal;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="WeaponStats")
	int32 MaxAmmoInClip;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	FAmmoData Ammo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=WeaponStats)
	int32 Damage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=WeaponStats)
	float ReloadDuration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category=WeaponStats)
	int32 Range;

	UPROPERTY(EditAnywhere, Category="Gameplay|Projectile")
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFiring = false;

#pragma  endregion

#pragma region Reloading
	// [server]
	UFUNCTION(BlueprintCallable)
	void RestoreAmmo();

	UFUNCTION(BlueprintCallable)
	bool CanFire() const;

	// IReloadable Interface.
	virtual bool CanReload() const override;

	// [server]
	UFUNCTION(BlueprintCallable)
	virtual void Reload() override;
	// ~IReloadable Interface

	UPROPERTY(BlueprintReadOnly)
	bool bIsReloading = false;

	UFUNCTION(Client, Reliable)
	void UseAmmo();
#pragma endregion

#pragma region Picking
	virtual bool CanPickupBy(AArcher* Character) const override;

	//virtual void MulticastPickup_Implementation(AArcher* Character) override;

	virtual void ServerPickup(AArcher* Character) override;
#pragma endregion
};
