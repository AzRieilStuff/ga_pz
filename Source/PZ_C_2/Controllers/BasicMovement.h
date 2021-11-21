// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BasicMovement.generated.h"

class UCameraComponent;
UCLASS()
class PZ_C_2_API UBasicMovement : public UActorComponent
{
	GENERATED_BODY()

	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	bool bZoomingIn;
public:
	UBasicMovement();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	UPROPERTY()
	UCameraComponent* PossessedCamera;

	UPROPERTY()
	UMeshComponent* PossessedMesh;

	UPROPERTY()
	USpringArmComponent* PossessedCameraArm;

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void PitchCamera(float AxisValue);

	UFUNCTION()
	void YawCamera(float AxisValue);

	UFUNCTION()
	void ZoomIn();

	UFUNCTION()
	void ZoomOut();

	UFUNCTION()
	void Jump();
public:
};
