// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableInterface.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class ABaseItem;
class AArcher;
class UPickBoxComponent;

DECLARE_DELEGATE_TwoParams(FCharacterItemInteraction, class ABaseItem*, AArcher*);

UCLASS()
class PZ_C_2_API ABaseItem : public AActor, public IPickableInterface
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	UPROPERTY(VisibleDefaultsOnly) 
	bool bDestroyOnPickup;
		
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	UPickBoxComponent* PickBoxComponent;

public:
	UFUNCTION()
	virtual void Pickup(AArcher* Character) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastPickup(AArcher* Character);
	
	UFUNCTION(Server, Reliable)
	virtual void ServerPickup(AArcher* Character);

	UFUNCTION(BlueprintCallable)
	virtual bool CanPickupBy(AArcher* Character) const override;

	UPROPERTY(BlueprintReadWrite)
	bool bPickable = true;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	FCharacterItemInteraction FOnItemPicked;
};
