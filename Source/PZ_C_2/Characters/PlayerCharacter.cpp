// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PZ_C_2/Inventory/Inventory.h"
#include "PZ_C_2/Movement/BaseMovementComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& OI) : Super(OI)
{
	// Setup arm component
	SpringArmComp = OI.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;

	// Setup camera
	CameraComp = OI.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	// Setup movement
	MovementComp = OI.CreateDefaultSubobject<UBaseMovementComponent>(this, "Movement");
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Setup inventory
	InventoryComponent = OI.CreateDefaultSubobject<UInventory>(this,TEXT("Inventory"));

	// Setup material
	MeshComp->SetMaterial(0, DefaultMaterial);

	// Setup weapon
	
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

UPawnMovementComponent* APlayerCharacter::GetMovementComponent() const
{
	return MovementComp;
}

UInventory* APlayerCharacter::GetInventory() const
{
	return InventoryComponent;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMovementComponent()->SetUpdatedComponent(RootComponent);

}
