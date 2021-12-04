// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlagueCube.generated.h"

UENUM()
enum class CubeState // no real implementation of it
{
	ACTIVE,
	PASSIVE
};

/**
 * 
 */
UCLASS()
class PZ_C_2_API APlagueCube : public AActor
{
	GENERATED_BODY()
public:
	APlagueCube();
	~APlagueCube();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnter( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnExit(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/** Delegate for notification when a wake event is fired by physics*/
private:
	UPROPERTY()
	TMap<class AStaticMeshActor*, CubeState> Cubes;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Collider;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* PlagueMesh;

	UPROPERTY()
	FTimerHandle SpawnHandler;
	UPROPERTY()
	FTimerHandle StateChangeHandler;

	UFUNCTION()
	void FillWithMeshes(int MeshCount);

	UFUNCTION()
	void ClearMeshes(int MeshCount);


	AStaticMeshActor* SpawnMeshItem();
};
