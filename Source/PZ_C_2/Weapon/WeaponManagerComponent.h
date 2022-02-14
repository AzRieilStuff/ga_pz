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

	// [client]
	UFUNCTION()
	virtual void EquipWeapon(ABaseRangeWeapon* NewWeapon);

	// [server]
	UFUNCTION()
	virtual void EquipWeaponFromClass(TSubclassOf<ABaseRangeWeapon> WeaponClass);

	// [client]
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

	// arming & disarming
private:
	FTimerHandle DisarmTimer;
	FTimerHandle ArmTimer;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	bool bIsWeaponArmed;
public:
	const FName BowArmSocket = FName("BowSocket");

	const FName BowBackSocket = FName("SpineBowSocket");

	const FName QuiverBackSocket = FName("SpineQuiverSocket");

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool IsWeaponArmed() const { return bIsWeaponArmed; } 

	void OnToggleArmAction();

	UFUNCTION(Server, Unreliable)
	void ServerDisarmWeapon();
	
	UFUNCTION(Server, Unreliable)
	void ServerArmWeapon();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastDisarmWeapon();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastArmWeapon();
	
	void DisarmWeapon();

	void OnDisarmTimerEnds();

	void ArmWeapon();
	
	void OnArmTimerEnds();

	// [client] calls from anim notify event
	UFUNCTION(BlueprintCallable)
	void OnDisarmWeaponPlaced();
	
	// [client] calls from anim notify event
	UFUNCTION(BlueprintCallable)
	void OnArmWeaponPlaced();
	// ~arming & disarming end
};
