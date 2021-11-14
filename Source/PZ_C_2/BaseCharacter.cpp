// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABaseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABaseCharacter::TouchStopped);
}

void ABaseCharacter::RestoreFullHealth()
{
	SetHealth(MaxHealth);
}

void ABaseCharacter::RestoreHealth(const float HealthAmount)
{
	SetHealth(Health + HealthAmount);
}

void ABaseCharacter::SetHealth(const float HealthAmount)
{
	const float UpdateValue = FMath::Max(0.f, FMath::Min(HealthAmount, MaxHealth));
	if (UpdateValue != Health)
	{
		OnHealthChangeMulticastDelegate.Broadcast(UpdateValue);
	}

	Health = UpdateValue;
	if (Health == 0)
	{
		OnDeathDelegate.ExecuteIfBound(UpdateValue);
		Kill();
	}
}

void ABaseCharacter::Kill()
{
	GetCharacterMovement()->DisableMovement();

	FTimerHandle KillTimer;
	GetWorldTimerManager().SetTimer(KillTimer, [&]
	{
		Destroy();
	}, 3.f, false);
}

void ABaseCharacter::SetRegeneration(const float RegenerationRate, const int32 Ticks)
{
	GetWorldTimerManager().ClearTimer(RegenerationTimerHandle);
	if (RegenerationRate == 0 || Ticks == 0)
	{
		return;
	}

	if (Ticks == 1)
	{
		RestoreHealth(RegenerationRate);
		return;
	}

	RegenerationTicks = 1;
	// When is it acceptable to use auto? - When you need to bind a lambda to a variable, as lambda types are not expressible in code.
	auto Regeneration = [this, Ticks, RegenerationRate]  
	{
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Green, FString::FromInt(RegenerationTicks));
		RestoreHealth(RegenerationRate);
		if (RegenerationTicks++ >= Ticks)
		{
			GetWorldTimerManager().ClearTimer(RegenerationTimerHandle);
		}
	};

	GetWorldTimerManager().SetTimer(RegenerationTimerHandle, Regeneration, RegenerationInterval, true, 0.f);
}


void ABaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ABaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// find weapon
	TArray<AActor*> Childs;
	GetAllChildActors(Childs, true);
	Childs.FindItemByClass<ABaseWeapon>(&Weapon);
}

bool ABaseCharacter::PickItem(ABaseItem* Item)
{
	Item->UseItem(this);
	return true;
}
