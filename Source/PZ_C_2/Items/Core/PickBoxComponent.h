// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableInterface.h"
#include "Components/BoxComponent.h"
#include "PickBoxComponent.generated.h"

class ABaseItem;
/**
 * 
 */
UCLASS()
class PZ_C_2_API UPickBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

	UPROPERTY()
	ABaseItem* Item;
public:
	UPickBoxComponent();

	virtual void BeginPlay() override;

	// [client]
	void EnablePhysics();

	// [client]
	void DisablePhysics();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
