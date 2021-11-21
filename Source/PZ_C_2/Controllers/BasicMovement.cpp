// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicMovement.h"
#include "Camera/CameraComponent.h"

UBasicMovement::UBasicMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBasicMovement::TickComponent(float DeltaTime, ELevelTick Tick, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	if (bZoomingIn)
	{
		ZoomFactor += DeltaTime / 0.5f;
	}
	else
	{
		ZoomFactor -= DeltaTime / 0.25f;
	}
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);

	PossessedCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
	PossessedCameraArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);

	// Rotate mesh
	{
		FRotator NewRotation = PossessedMesh->GetComponentRotation();
		NewRotation.Yaw += CameraInput.X;
		PossessedMesh->SetWorldRotation(NewRotation);
	}

	// Rotate arm
	{
		FRotator NewRotation = PossessedCameraArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		NewRotation.Yaw += CameraInput.X;
		PossessedCameraArm->SetWorldRotation(NewRotation);
	}

	// Movement
	{
		if (!MovementInput.IsZero())
		{
			MovementInput = MovementInput.GetSafeNormal() * 100.0f;
			FVector NewLocation = PossessedMesh->GetComponentLocation();
			NewLocation += PossessedMesh->GetForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += PossessedMesh->GetRightVector() * MovementInput.Y * DeltaTime;
			PossessedMesh->SetWorldLocation(NewLocation);
		}
	}
}

void UBasicMovement::BeginPlay()
{
	Super::BeginPlay();
}

void UBasicMovement::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void UBasicMovement::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void UBasicMovement::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void UBasicMovement::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void UBasicMovement::ZoomIn()
{
	bZoomingIn = true;
}

void UBasicMovement::ZoomOut()
{
	bZoomingIn = false;
}

void UBasicMovement::Jump()
{
	PossessedMesh->AddForce(PossessedMesh->GetUpVector() * 50000.f * PossessedMesh->GetMass());
}
