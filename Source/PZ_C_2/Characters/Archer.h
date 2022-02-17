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

UENUM(Blueprintable, Meta = (Bitflags))
enum class ECharacterStateFlags : uint8
{
	Firing = 0,
	// old

	DisarmingBow,
	ArmingBow,
	Aiming,
	AimReady
};

ENUM_CLASS_FLAGS(ECharacterStateFlags);


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

private:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline UCameraComponent* GetCameraComponent() const { return CameraComponent; };

	UPROPERTY(BlueprintReadOnly)
	FVector LocalVelocity;
protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
public:
	UPROPERTY(EditDefaultsOnly)
	float MaxPitchRotation;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Components & managers
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated)
	UWeaponManagerComponent* WeaponManagerComponent;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UInventoryManagerComponent* InventoryManagerComponent;
	// ~Components

	// Climbing // todo remove or upgade
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
	// ~Health 

	UFUNCTION(BlueprintCallable, Category = "Stats")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	virtual bool CanJumpInternal_Implementation() const override;

	// Movement
private:
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCharacterMovementComponent* GetCharacterMovementComponent() const;
	// ~Movement

	// ~Health implementation

	// Saving
	FCharacterSaveData GetSaveData() const;

	void InitFromSaveData(const FCharacterSaveData Data);
	// ~Saving

	// [server] Weapon interaction
	UFUNCTION()
	void EquipDefaultWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmingDuration;
	// ~Weapon interaction

	// States
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
		meta = (Bitmask, BitmaskEnum = "ECharacterStateFlags", AllowPrivateAccess="true"),
		ReplicatedUsing=OnRep_StateFlags)
	int32 StateFlags;


	UFUNCTION()
	void OnRep_StateFlags(const int32 PrevValue);
public:
	UFUNCTION(BlueprintCallable)
	void SetState(ECharacterStateFlags Flag);

	UFUNCTION(BlueprintCallable)
	void ClearState(ECharacterStateFlags Flag);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasState(ECharacterStateFlags Flag) const;

	bool HasState(ECharacterStateFlags Flag, int32 BitMask) const;

	// ~States
};
