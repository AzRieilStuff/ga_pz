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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UPickBoxComponent* PickBoxComponent;

public:
	inline UPickBoxComponent* GetPickBoxComponent() const { return PickBoxComponent; };

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma region Dropping
public:
	void OnDropped();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	           UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
#pragma endregion
#pragma region Inventory data
public:
	UPROPERTY(EditDefaultsOnly)
	UTexture* InventoryIcon;


#pragma endregion
#pragma region Storing
protected:
	static int32 MaxPerStack;

	static bool bStackable;

public:
	UFUNCTION(BlueprintCallable)
	static inline int32 GetStackLimit() { return MaxPerStack; };

	// can be stacked with other items of this type
	UFUNCTION(BlueprintCallable)
	static inline int32 GetIsStackable() { return bStackable; };

	UFUNCTION()
	virtual UBaseInventoryItem* GenerateInventoryData(UBaseInventoryItem* Target = nullptr) const;

	void OnStored();
#pragma endregion
#pragma region Picking
private:
protected:
	bool bPickable = true;

	UPROPERTY(EditAnywhere)
	bool bDestroyOnPickup;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool GetIsPickable() const { return bPickable; }

	// [client]
	UFUNCTION()
	virtual void TryPickup(AArcher* Character) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastPickup(AArcher* Character);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerPickup(AArcher* Character);

	UFUNCTION(BlueprintCallable)
	virtual bool CanPickupBy(AArcher* Character) const override;
#pragma endregion
};
