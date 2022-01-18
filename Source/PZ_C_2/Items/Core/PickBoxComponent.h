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

	//void OnDeactivate( UActorComponent* Component );

	//UFUNCTION()
	//void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
