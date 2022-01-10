// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPCharacter.generated.h"

class ABaseWeapon;

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
class PZ_C_2_API ATPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATPCharacter();

	UPROPERTY()
	class UChildActorComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon> DefaultWeapon;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SetWeaponFromActor(ABaseWeapon* NewWeapon);

	UFUNCTION()
	void SetWeaponFromClass(TSubclassOf<ABaseWeapon> NewWeapon);

	UFUNCTION(BlueprintCallable)
	ABaseWeapon* GetWeapon() const;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	class UInventory* InventoryComponent;

	UFUNCTION()
	void Climb();

	UFUNCTION(Server, Unreliable)
	void ClimbServer();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayClimbAnimationMulticast();

	UPROPERTY(BlueprintReadOnly)
	bool IsClimbing;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ClimbingMontage;

	// Health implementation
	FOnHealthChangeDelegate OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeDynamicDelegate OnHealthChangeDynamic;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	UFUNCTION()
	void OnRep_CurrentHealth();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnHealthUpdate();

	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	                 AActor* DamageCauser) override;

	// ~Health implementation

	// Saving
	FCharacterSaveData GetSaveData() const;

	void InitFromSaveData(const FCharacterSaveData Data);
	// ~Saving
};
