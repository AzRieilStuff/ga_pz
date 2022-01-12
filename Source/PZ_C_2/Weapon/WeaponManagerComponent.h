﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRangeWeapon.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class AArcher;

/**
 * 
 */
UCLASS()
class PZ_C_2_API UWeaponManagerComponent : public UActorComponent
{
	// temporary solution
	void SetBowMeshVisibility(bool State) const;

	const FName BowSocketName = FName("BowSocket");

	GENERATED_BODY()

public:
	UWeaponManagerComponent();

	UFUNCTION()
	virtual void EquipWeapon(ABaseRangeWeapon* NewWeapon);

	UFUNCTION()
	void UnequipWeapon();

	UFUNCTION()
	void InteractWeapon();

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE bool IsFiring() { return Weapon && Weapon->bIsFiring; };

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE bool IsWeaponEquipped() { return Weapon != nullptr; };

	UPROPERTY(BlueprintReadOnly)
	ABaseRangeWeapon* Weapon;

	UPROPERTY()
	AArcher* Character;

	virtual void BeginPlay() override;
};