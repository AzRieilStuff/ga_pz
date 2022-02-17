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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=WeaponStats)
	float FireRate;

#pragma region Aiming
private:
	FTimerHandle AimingTimer;

	FTimerHandle FiringTimer;

	UFUNCTION(Server, Unreliable)
	void ServerStartAiming();

	void StartAiming();
#pragma endregion

#pragma region Firing
	// [local + server] run animation & timer
	void StartShootingTimer();

	void BreakShootingTimer();

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

	UFUNCTION(BlueprintCallable)
	virtual void FireAction();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerFireAction();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastFireAction();

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

#pragma region Fire interrupt
	UFUNCTION(Server, Unreliable)
	virtual void ServerInterruptFire();

	UFUNCTION(NetMulticast, Unreliable)
	virtual void MulticastInterruptFire();

	virtual void InterruptFire();
#pragma endregion

#pragma region Picking
	virtual bool CanPickupBy(AArcher* Character) const override;

	//virtual void MulticastPickup_Implementation(AArcher* Character) override;

	virtual void ServerPickup(AArcher* Character) override;
#pragma endregion
};
