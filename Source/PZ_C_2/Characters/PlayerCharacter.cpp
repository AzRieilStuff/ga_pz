// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PZ_C_2/Controllers/BasicMovement.h"
#include "PZ_C_2/Inventory/Inventory.h"

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
	//SpringArmComp->bUsePawnControlRotation = true; 

	// Setup camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//ameraComp->bUsePawnControlRotation = true;

	// Setup movement
	MovementComp = CreateDefaultSubobject<UBasicMovement>("Movement");
	MovementComp->PossessedCamera = CameraComp;
	MovementComp->PossessedMesh = MeshComp;
	MovementComp->PossessedCameraArm = SpringArmComp;

	// Setup inventory
	InventoryComponent = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));

	// Setup input
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, MovementComp, &UBasicMovement::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Released, MovementComp, &UBasicMovement::ZoomOut);
	InputComponent->BindAction("Jump", IE_Pressed, MovementComp, &UBasicMovement::Jump);

	InputComponent->BindAxis("MoveForward", MovementComp, &UBasicMovement::MoveForward);
	InputComponent->BindAxis("MoveRight", MovementComp, &UBasicMovement::MoveRight);
	InputComponent->BindAxis("LookUp", MovementComp, &UBasicMovement::PitchCamera);
	InputComponent->BindAxis("Turn", MovementComp, &UBasicMovement::YawCamera);
}

UInventory* APlayerCharacter::GetInventory() const
{
	return InventoryComponent;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
