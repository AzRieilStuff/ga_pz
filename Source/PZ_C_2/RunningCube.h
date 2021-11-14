// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunningCube.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangeMulticastDelegate, const bool, State);
	
UCLASS()
class PZ_C_2_API ARunningCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunningCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;


	UFUNCTION()
	void Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Deactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void Jump();

	UFUNCTION()
	void OnOverlapStateChange(bool State);

	UPROPERTY(BlueprintAssignable)
	FOnStateChangeMulticastDelegate OnStateChangeMulticastDelegate;

public:	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UMaterialInstance* ActiveStateMaterial;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UMaterialInstance* IdleStateMaterial;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};

