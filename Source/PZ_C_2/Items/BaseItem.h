// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class ABaseItem;
class ATPCharacter;

DECLARE_DELEGATE_TwoParams(FCharacterItemInteraction, class ABaseItem*, ATPCharacter* );	
UCLASS()
class PZ_C_2_API ABaseItem : public AActor
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseItem(ATPCharacter* Character);

	UPROPERTY(BlueprintReadWrite)
	bool bPickable = true;

	UFUNCTION(BlueprintCallable)
	virtual bool CanUseBy(ATPCharacter* Character);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	FCharacterItemInteraction FOnItemPicked;
};

