// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableInterface.h"
#include "GameFramework/Actor.h"
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"
#include "BaseItem.generated.h"

class ABaseItem;
class AArcher;
class UPickBoxComponent;

//DECLARE_DELEGATE_TwoParams(FCharacterItemInteraction, class ABaseItem*, AArcher*);

UCLASS()
class PZ_C_2_API ABaseItem : public AActor, public IPickableInterface
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	UPROPERTY(EditAnywhere)
	bool bDestroyOnPickup;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UPickBoxComponent* PickBoxComponent;

public:
	UFUNCTION()
	virtual void GenerateInventoryData(FInventoryItem& InventoryData) const;

	UFUNCTION()
	virtual void Pickup(AArcher* Character) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastPickup(AArcher* Character);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerPickup(AArcher* Character);

	UFUNCTION(BlueprintCallable)
	virtual bool CanPickupBy(AArcher* Character) const;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture* InventoryIcon;

	/**
	 * @brief Item can be picked
	 */
	UPROPERTY(BlueprintReadWrite)
	bool bPickable = true;

	/**
	 * @brief Item will be stored into inventory
	 */
	UPROPERTY()
	bool bStoreable;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//FCharacterItemInteraction FOnItemPicked;
};
