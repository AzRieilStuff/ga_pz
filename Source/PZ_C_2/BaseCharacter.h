// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Inventory/IContainer.h"
#include "Items/BaseItem.h"
#include "Weapon/BaseWeapon.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDeath, const float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnHealthChangeMulticastDelegate, const float, Health);

UCLASS(config=Game)
class ABaseCharacter : public ACharacter
{
private:
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	FTimerHandle RegenerationTimerHandle;
	int32 RegenerationTicks;
public:
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadOnly)
	ABaseWeapon* Weapon;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeMulticastDelegate OnHealthChangeMulticastDelegate;
	
	FOnDeath OnDeathDelegate;

	virtual void BeginPlay() override;

	UFUNCTION()
	bool PickItem(ABaseItem* Item);

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	UPROPERTY(BlueprintReadWrite)
	float Health;
	
	UPROPERTY(BlueprintReadWrite)
	float MaxHealth;
	
	UFUNCTION(BlueprintCallable)
	void RestoreFullHealth();

	UFUNCTION(BlueprintCallable)
	void RestoreHealth( const float HealthAmount );

	UFUNCTION(BlueprintCallable)
	void SetHealth( const float HealthAmount );

	UFUNCTION(BlueprintCallable)
	void Kill();

	UFUNCTION(BlueprintCallable)
	void SetRegeneration( const float RegenerationRate, const int32 Ticks);

	UPROPERTY(BlueprintReadWrite)
	float RegenerationInterval = 2.f;

	
	/** Returns CameraBoom subobject **/	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
