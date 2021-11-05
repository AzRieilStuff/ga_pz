// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IReloadable.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS(Blueprintable)
class PZ_C_2_API UBaseWeapon : public USkeletalMeshComponent, public IReloadable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UBaseWeapon();

protected:
		
public:	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxAmmoTotal;

	UPROPERTY(BlueprintReadWrite)
	int32 AmmoTotal;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxAmmoInClip;

	UPROPERTY(BlueprintReadWrite)
	int32 AmmoInClip;
	
	UPROPERTY(BlueprintReadWrite)
	int32 Damage;

	UPROPERTY(BlueprintReadWrite)
	float ReloadDuration;

	UPROPERTY(BlueprintReadWrite)
	int Range;

	UPROPERTY(BlueprintReadWrite)
	FName MuzzleSocketName = "Muzzle";

	UFUNCTION(BlueprintCallable)
	void Fire();

	virtual bool CanReload() const override;

	UFUNCTION(BlueprintCallable)
	bool CanFire() const;

    UFUNCTION(BlueprintCallable)
	virtual void Reload() override;

	UPROPERTY(BlueprintReadOnly)
	bool IsReloading = false;

	void UseAmmo();

	void WeaponTrace(FVector from, FVector to);
};
