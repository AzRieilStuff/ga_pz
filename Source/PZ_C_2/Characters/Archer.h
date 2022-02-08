// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PZ_C_2/Weapon/WeaponManagerComponent.h"
#include "Archer.generated.h"

class ABaseRangeWeapon;

DECLARE_MULTICAST_DELEGATE(FOnHealthChangeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChangeDynamicDelegate);

USTRUCT()
struct FCharacterSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	float Health;

	UPROPERTY()
	FVector Location;
	
	UPROPERTY()
	FQuat Rotation;
};

UCLASS()
class PZ_C_2_API AArcher : public ACharacter
{
	GENERATED_BODY()

public:
	AArcher();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UChildActorComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABaseRangeWeapon> DefaultWeapon;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	UAnimMontage* ClimbingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UWidgetComponent* TopBar;

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
public:

	UPROPERTY(EditDefaultsOnly)
	float MaxPitchRotation;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Weapon
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated)
	UWeaponManagerComponent* WeaponManagerComponent;

	// ~Weapon

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	class UInventoryManagerComponent* InventoryManagerComponent;

	// Climbing
	UFUNCTION()
	void Climb();

	UFUNCTION(Server, Unreliable)
	void ClimbServer();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayClimbAnimationMulticast();

	UPROPERTY(BlueprintReadOnly)
	bool IsClimbing;
	// ~Climbing

	// Health implementation
	FOnHealthChangeDelegate OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeDynamicDelegate OnHealthChangeDynamic;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	UFUNCTION()
	void OnRep_CurrentHealth();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnHealthUpdate();

	UFUNCTION(BlueprintPure, Category="Stats")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category="Stats")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category="Stats")
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	                 AActor* DamageCauser) override;

	
	// ~Health implementation

	// Saving
	FCharacterSaveData GetSaveData() const;

	void InitFromSaveData(const FCharacterSaveData Data);
	// ~Saving

	// Firing

	UPROPERTY()
	bool bIsFiring;

	UFUNCTION()
	void EquipDefaultWeapon();

};
