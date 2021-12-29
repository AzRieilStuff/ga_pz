// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IReloadable.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

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
class PZ_C_2_API ABaseWeapon : public AActor, public IReloadable
{
	GENERATED_BODY()

public:
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	USkeletalMeshComponent* MeshComponent;
public:
	ABaseWeapon();

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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MuzzleSocketName;

	UFUNCTION(BlueprintCallable)
	void TryFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void Fire();

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
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

	UFUNCTION(Client, Reliable)
	void UseAmmo();

	FHitResult WeaponTrace(FVector& From, FVector& To);
};
