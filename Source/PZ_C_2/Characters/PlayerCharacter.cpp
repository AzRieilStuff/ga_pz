// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PZ_C_2/Environment/SpyCamera.h"
#include "PZ_C_2/Inventory/Inventory.h"
#include "PZ_C_2/Movement/BaseMovementComponent.h"

void APlayerCharacter::OnSpyDetected()
{
	GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::Red, TEXT("Found"));
	MeshComp->SetMaterial(0, DetectedMaterial);
}

void APlayerCharacter::OnSpyForget()
{
	GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::Red, TEXT("Lost"));
	MeshComp->SetMaterial(0, DefaultMaterial);
}

APlayerCharacter::APlayerCharacter()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Setup arm component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;

	// Setup camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	// Setup movement
	MovementComp = CreateDefaultSubobject<UBaseMovementComponent>("Movement");
	MovementComp->SetUpdatedComponent(MeshComp);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Setup inventory
	InventoryComponent = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));

	// Setup material
	MeshComp->SetMaterial(0, DefaultMaterial);
	
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

	// SpyCamera
	for (TActorIterator<ASpyCamera> It(GetWorld()); It; ++It)
	{
		(*It)->OnCharacterFound.BindUFunction(this, "OnSpyDetected");
		(*It)->OnCharacterLost.BindUFunction(this, "OnSpyForget");
	}
}
