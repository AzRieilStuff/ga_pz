// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SpyCamera.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnCharacterFound);
DECLARE_DYNAMIC_DELEGATE(FOnCharacterLost);

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PZ_C_2_API ASpyCamera : public AActor
{
private:
	UPROPERTY()
	FTimerHandle FoundTimer;
	
	UPROPERTY()
	FTimerHandle ForgetTimer;

	UPROPERTY()
	APawn* DetectedPawn;

	UFUNCTION()
	void RememberPawn(APawn* Pawn);

	UFUNCTION()
	void ForgetPawn();
public:
	GENERATED_BODY()

	UPROPERTY()
	float FoundDelay = 5.f;

	UPROPERTY()
	FOnCharacterFound OnCharacterFound;
	UPROPERTY()
	FOnCharacterLost OnCharacterLost;

	UPROPERTY()
	float ForgetDelay = 3.f;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxTrigger;

	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

	ASpyCamera();
};
