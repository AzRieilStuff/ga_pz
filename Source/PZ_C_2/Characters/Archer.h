// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
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
class PZ_C_2_API AArcher : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArcher();

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	virtual float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

#pragma region Components
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Replicated)
	UWeaponManagerComponent* WeaponManagerComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UAbilitySystemComponent* AbilitySystemComponent;

	inline virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	};
#pragma endregion

#pragma region GAS
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UCharacterAttributeSet* Attributes;
#pragma endregion

#pragma region Camera
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	float MaxPitchRotation;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComponent;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline UCameraComponent* GetCameraComponent() const { return CameraComponent; };

	UPROPERTY(EditDefaultsOnly)
	float CameraDistanceDefault;

	UPROPERTY(EditDefaultsOnly)
	float CameraDistanceAiming;

	UPROPERTY(EditDefaultsOnly)
	FVector CameraOffsetDefault;

	UPROPERTY(EditDefaultsOnly)
	FVector CameraOffsetAiming;

	float CameraDistanceCurrent;
	FVector CameraOffsetCurrent;
#pragma endregion

#pragma region Climbing
public:
	UPROPERTY(BlueprintReadOnly)
	bool IsClimbing;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	UAnimMontage* ClimbingMontage;

	UFUNCTION()
	void Climb();

	UFUNCTION(Server, Unreliable)
	void ClimbServer();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayClimbAnimationMulticast();

#pragma endregion

#pragma region Movement
private:
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;
public:
	virtual bool CanJumpInternal_Implementation() const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCharacterMovementComponent* GetCharacterMovementComponent() const;
#pragma endregion

#pragma region Weapon interaction
public:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABaseRangeWeapon> DefaultWeapon;

	// [server]
	UFUNCTION()
	void EquipDefaultWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmingDuration;
private:
	UPROPERTY()
	class UChildActorComponent* WeaponComponent;

#pragma endregion

#pragma region Character states
	/*
	 * states are using for animation replication, not qualified to be smth rely on
	 * will be replicated if changed from server of from owner
	*/
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

	UFUNCTION(Server, Unreliable)
	void ServerSetState(ECharacterStateFlags Flag);

	UFUNCTION(BlueprintCallable)
	void ClearState(ECharacterStateFlags Flag);

	UFUNCTION(Server, Unreliable)
	void ServerClearState(ECharacterStateFlags Flag);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasState(ECharacterStateFlags Flag) const;

	bool HasState(ECharacterStateFlags Flag, int32 BitMask) const;

#pragma endregion
};
