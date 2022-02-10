// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRangeWeapon.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class AArcher;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponInteraction, ABaseRangeWeapon*, Item);

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

protected:
	// [server]
	void SetCurrentWeapon(ABaseRangeWeapon* Weapon, ABaseRangeWeapon* PrevWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(ABaseRangeWeapon* Weapon);
	
	UFUNCTION(Server, Reliable)
	void ServerUnequipWeapon();
public:
	UWeaponManagerComponent();

	// [local]
	UFUNCTION()
	virtual void EquipWeapon(ABaseRangeWeapon* NewWeapon);

	// [server]
	UFUNCTION()
	virtual void EquipWeaponFromClass(TSubclassOf<ABaseRangeWeapon> WeaponClass);

	// [local]
	UFUNCTION()
	void UnequipWeapon();

	UFUNCTION()
	void InteractWeapon();

	UFUNCTION(BlueprintCallable)
	void OnReloadAction();

	UFUNCTION(Server, Unreliable)
	void ServerReloadCurrentWeapon();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE bool IsFiring() { return CurrentWeapon && CurrentWeapon->bIsFiring; };

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE bool IsWeaponEquipped() { return CurrentWeapon != nullptr; };

	UPROPERTY(Transient, BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing=OnRep_CurrentWeapon)
	ABaseRangeWeapon* CurrentWeapon = nullptr;

	UFUNCTION()
	void OnRep_CurrentWeapon(ABaseRangeWeapon* PrevWeapon);

	UPROPERTY(Replicated)
	AArcher* Character;

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	bool CanEquipWeapon(const ABaseRangeWeapon* NewWeapon) const;
	
	UPROPERTY(BlueprintAssignable)
	FOnWeaponInteraction OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponInteraction OnWeaponUnequipped;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
