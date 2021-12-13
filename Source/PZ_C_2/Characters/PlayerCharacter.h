// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PZ_C_2/Inventory/IContainer.h"
#include "PlayerCharacter.generated.h"

class UBaseMovementComponent;
class UInventory;

/**
 * 
 */
UCLASS()
class PZ_C_2_API APlayerCharacter : public ABaseCharacter, public IContainer
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	float Stamina;

public:
	UPROPERTY(EditAnywhere)
	UMaterialInstance* DefaultMaterial;
	
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UInventory* InventoryComponent;
	
	APlayerCharacter(const FObjectInitializer& OI);

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
	UBaseMovementComponent* MovementComp;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;
	
	// IContainer
	UFUNCTION(BlueprintCallable)
	virtual UInventory* GetInventory() const override;

	// IContainer end
	virtual void BeginPlay() override;
};