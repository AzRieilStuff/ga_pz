// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableInterface.h"
#include "GameFramework/Actor.h"
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"
#include "BaseItem.generated.h"

class AArcher;
class UPickBoxComponent;
class UBaseInventoryItem;

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
	inline UPickBoxComponent* GetPickBoxComponent() const { return PickBoxComponent; };

	UFUNCTION()
	virtual UBaseInventoryItem* GenerateInventoryData(UBaseInventoryItem* Target = nullptr) const;

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
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bPickable = true;

	/**
	 * @brief Item will be stored into inventory
	 */
	UPROPERTY(VisibleAnywhere)
	bool bStoreable;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPerStack;

	void OnStored();

	// [server]
	//virtual void InitFromInventory(UBaseInventoryItem* Item);
};
