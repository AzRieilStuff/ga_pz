// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
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

UENUM()
enum class EAbility : uint8
{
	Aim,
	Shoot,
	Jump,
	Sprint
};

// Animation states that are not part of GAS
UENUM(Blueprintable, Meta = (Bitflags))
enum class ECharacterStateFlags : uint8
{
	NONE = 0,
	// old

	DisarmingBow,
	ArmingBow,
	Aiming,
};

ENUM_CLASS_FLAGS(ECharacterStateFlags);


UCLASS()
class PZ_C_2_API AArcher : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArcher();

	// [server]
	virtual void PossessedBy(AController* NewController) override;

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
private:
	UPROPERTY(Replicated)
	UWeaponManagerComponent* WeaponManagerComponent;

	UPROPERTY(EditDefaultsOnly)
	class UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent;
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline UWeaponManagerComponent* GetWeaponManagerComponent() const
	{
		return WeaponManagerComponent;
	};

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline UInventoryManagerComponent* GetInventoryManagerComponent() const
	{
		return InventoryManagerComponent;
	};

	inline virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	};
#pragma endregion

#pragma region GAS
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UCharacterAttributeSet* Attributes;

	void GrantDefaultAbilities();

	void ApplyDefaultEffects();

	// [client]
	void InitClientAbilityMap();

	// storing abilities that exists only in one instance per class, used for easier access to GAS spec handle
	TMap<EAbility, FGameplayAbilitySpecHandle> AbilitiesMap;
public:
	// abilities
	FGameplayAbilitySpecHandle* GetAbilityHandleByKey(const EAbility Key);

	FGameplayAbilitySpec* GetAbilitySpecByKey(const EAbility Key);

	UFUNCTION(BlueprintCallable)
	bool HasActiveAbility(const EAbility Key);

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TMap<EAbility, TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayEffect>> ApplyEffectsOnStartup;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmingDuration;
private:
	UPROPERTY()
	class UChildActorComponent* WeaponComponent;

#pragma endregion

#pragma region Character states
	// states are using mostly for animation replication, not reliable much
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

#pragma region Projectile sticking
private:
	TArray<FName> ProjectileStickSocketNames;

	void LocateStickSockets();
public:
	FName* FindClosestSocket(const FVector Position);
#pragma endregion
};
