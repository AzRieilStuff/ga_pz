// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class ABaseItem;
class ABaseWeapon;
class IContainer;

USTRUCT()
struct FCharacterSTats
{
	GENERATED_BODY()
	float Health;
	float MaxHealth;
	
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDeath, const float, Health);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeMulticastDelegate, const float, Health);

UCLASS(config=Game)
class ABaseCharacter : public APawn
{
private:
	GENERATED_BODY()

	FTimerHandle RegenerationTimerHandle;
	int32 RegenerationTicks;
public:
	ABaseCharacter(const FObjectInitializer& OI);

	UPROPERTY(BlueprintReadOnly)
	ABaseWeapon* Weapon;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangeMulticastDelegate OnHealthChangeMulticastDelegate;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

	FOnDeath OnDeathDelegate;

	virtual void BeginPlay() override;

	UFUNCTION()
	bool PickItem(ABaseItem* Item);

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth;
	
	UFUNCTION(BlueprintCallable)
	void RestoreFullHealth();

	UFUNCTION(BlueprintCallable)
	void RestoreHealth(const float HealthAmount);

	UFUNCTION(BlueprintCallable)
	void SetHealth(const float HealthAmount);
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(const float HealthAmount);
	
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable)
	void Kill();

	UFUNCTION(BlueprintCallable)
	void SetRegeneration(const float RegenerationRate, const int32 Ticks);

	UPROPERTY(BlueprintReadWrite)
	float RegenerationInterval = 2.f;
};