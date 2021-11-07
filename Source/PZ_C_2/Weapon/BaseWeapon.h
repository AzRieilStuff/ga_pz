// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IReloadable.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

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

	UPROPERTY(BlueprintReadWrite)
	int32 AmmoTotal;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 MaxAmmoInClip;

	UPROPERTY(BlueprintReadWrite)
	int32 AmmoInClip;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Damage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ReloadDuration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Range;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName MuzzleSocketName;

	UFUNCTION(BlueprintCallable)
	void Fire();

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

	void UseAmmo();

	void WeaponTrace(FVector& From, FVector& To);
};
