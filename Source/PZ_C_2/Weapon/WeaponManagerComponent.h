// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRangeWeapon.h"
#include "Components/ActorComponent.h"
#include "WeaponManagerComponent.generated.h"

class AArcher;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponInteraction, ABaseRangeWeapon*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeAimState, bool, State);

/**
 * 
 */
UCLASS()
class PZ_C_2_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
	AArcher* Character;

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UWeaponManagerComponent();

#pragma region Equipping

protected:
	// [server]
	void SetCurrentWeapon(ABaseRangeWeapon* Weapon, ABaseRangeWeapon* PrevWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(ABaseRangeWeapon* Weapon);

	UFUNCTION(Server, Reliable)
	void ServerUnequipWeapon();

private:
	const FName BowArmSocket = FName("BowSocket");

	const FName BowBackSocket = FName("SpineBowSocket");

	const FName QuiverBackSocket = FName("SpineQuiverSocket");
	
	FTimerHandle DisarmTimer;
	FTimerHandle ArmTimer;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	bool bIsWeaponArmed;

public:
	// [client]
	UFUNCTION()
	virtual void EquipWeapon(ABaseRangeWeapon* NewWeapon);

	// [server]
	UFUNCTION()
	virtual void EquipWeaponFromClass(TSubclassOf<ABaseRangeWeapon> WeaponClass);

	// [client]
	UFUNCTION()
	void UnequipWeapon();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE bool IsWeaponEquipped() { return CurrentWeapon != nullptr; };

	UPROPERTY(Transient, BlueprintReadOnly, VisibleAnywhere, ReplicatedUsing=OnRep_CurrentWeapon)
	ABaseRangeWeapon* CurrentWeapon = nullptr;

	UFUNCTION()
	void OnRep_CurrentWeapon(ABaseRangeWeapon* PrevWeapon);

	UFUNCTION()
	bool CanEquipWeapon(const ABaseRangeWeapon* NewWeapon) const;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponInteraction OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponInteraction OnWeaponUnequipped;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWeaponArmed() const { return bIsWeaponArmed; }

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

#pragma endregion

#pragma region Firing
private:
	UPROPERTY(BlueprintAssignable, meta=(AllowPrivateAccess="true"))
	FOnChangeAimState OnChangeAimState;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	float AimingCameraTransitionDuration;

	FTimerHandle AimingCameraTimer;

	// cumulative value for camera animation time
	float CameraInterpTime;

	void CalcCameraPosition(FVector& Offset, float& Distance, const float Delta, const float InterpSpeed) const;
	void UpdateCameraPosition();
public:
	UFUNCTION()
	void OnFireAction();

	UFUNCTION()
	void OnFireReleasedAction();

	UFUNCTION()
	void OnInterruptFireAction();

	void SetAimCamera(const bool IsAim);
#pragma endregion

#pragma region Reloading
	UFUNCTION(BlueprintCallable)
	void OnReloadAction();

	UFUNCTION(Server, Unreliable)
	void ServerReloadCurrentWeapon();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE bool IsFiring() { return CurrentWeapon && CurrentWeapon->bIsFiring; };
#pragma endregion
};
