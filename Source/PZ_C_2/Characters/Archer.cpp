// Fill out your copyright notice in the Description page of Project Settings.

#include "Archer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "PZ_C_2/Weapon/BaseRangeWeapon.h"
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PZ_C_2/Ammo/Arrow.h"
#include "PZ_C_2/Framework/GameInstanceMain.h"

// Sets default values
AArcher::AArcher()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>("WeaponManager");
	WeaponManagerComponent->SetIsReplicated(true);

	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryComponent");
	InventoryManagerComponent->SetIsReplicated(true);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetIsReplicated(false);
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArmComponent->TargetOffset = FVector(0, 0, 75.f);
	SpringArmComponent->bEnableCameraLag = false;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	GetMesh()->SetIsReplicated(true); // replicate bone rotation
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	// Stats defaults
	MaxHealth = 100.0f;
	CurrentHealth = 90.f;
	ArmingDuration = 1.f;

	GetCharacterMovement()->JumpZVelocity = 800.f;
	//bNetUseOwnerRelevancy = true;

	// camera default settings
	CameraDistanceDefault = 300.f;
	CameraDistanceAiming = 50.f;
	CameraOffsetDefault = FVector(0, 0, 100.f);
	CameraOffsetAiming = FVector(0, 30.f, 50.f);

	MaxPitchRotation = 40.f;
}

// Called when the game starts or when spawned
void AArcher::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (CameraManager) // will be empty for AI or dedicated
	{
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = MaxPitchRotation;
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -MaxPitchRotation;
	}
}

void AArcher::BeginPlay()
{
	Super::BeginPlay();

	CameraDistanceCurrent = CameraDistanceDefault;
	CameraOffsetCurrent = CameraOffsetDefault;

	SpringArmComponent->TargetArmLength = CameraDistanceCurrent;
	SpringArmComponent->TargetOffset = CameraOffsetCurrent;

	// Init default weapon
	if (HasAuthority() && DefaultWeapon)
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &AArcher::EquipDefaultWeapon);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Equip default weapon");
	}

	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
}

void AArcher::EquipDefaultWeapon()
{
	WeaponManagerComponent->EquipWeaponFromClass(DefaultWeapon);
}

void AArcher::OnRep_StateFlags(const int32 PrevValue)
{
}

void AArcher::SetState(ECharacterStateFlags Flag)
{
	StateFlags |= 1 << static_cast<int32>(Flag);
}

void AArcher::ClearState(ECharacterStateFlags Flag)
{
	StateFlags &= ~(1 << static_cast<int32>(Flag));
}

bool AArcher::HasState(ECharacterStateFlags Flag) const
{
	return HasState(Flag, StateFlags);
}

bool AArcher::HasState(ECharacterStateFlags Flag, int32 BitMask) const
{
	return ((BitMask) & (1 << static_cast<int32>(Flag))) > 0;
}


void AArcher::SetCurrentHealth(float healthValue)
{
	// Update server value
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate(); // if server changes value, RepNotify wont call
	}
}

float AArcher::TakeDamage(float DamageTaken, const struct FDamageEvent& DamageEvent, AController* EventInstigator,
                          AActor* DamageCauser)
{
	Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);

	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);

	if (DamageCauser->IsA(AArrow::StaticClass()))
	{
		// sticking
		DamageCauser->SetOwner(this);
		DamageCauser->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	}
	return damageApplied;
}

bool AArcher::CanJumpInternal_Implementation() const
{
	return Super::CanJumpInternal_Implementation() && !HasState(ECharacterStateFlags::Firing);
}

UCharacterMovementComponent* AArcher::GetCharacterMovementComponent() const
{
	return CharacterMovementComponent;
}

FCharacterSaveData AArcher::GetSaveData() const
{
	return FCharacterSaveData{
		CurrentHealth,
		GetActorLocation(),
		GetActorRotation().Quaternion()
	};
}

void AArcher::InitFromSaveData(const FCharacterSaveData Data)
{
	CurrentHealth = Data.Health;
	SetActorLocation(Data.Location, false, nullptr, ETeleportType::ResetPhysics);
	SetActorRotation(Data.Rotation, ETeleportType::ResetPhysics);
}


void AArcher::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArcher, CurrentHealth);
	DOREPLIFETIME_CONDITION(AArcher, StateFlags, COND_SkipOwner);
}

void AArcher::OnHealthUpdate()
{
	if (IsLocallyControlled()) // update local
	{
		FString healthMessage = FString::Printf(TEXT("Player %s now has %f HP"), *GetFName().ToString(), CurrentHealth);

		auto PC = GetWorld()->GetFirstPlayerController();
		PC->ClientMessage(healthMessage);
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(
			TEXT("%s now has %f HP ( ROLE_Authority )"), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, healthMessage);
	}

	OnHealthChange.Broadcast();
	OnHealthChangeDynamic.Broadcast();
}

void AArcher::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

// Called every frame
void AArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UGameInstanceMain* GameInstance = Cast<UGameInstanceMain>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		PlayerInputComponent->BindAction("QuickSave", IE_Pressed, GameInstance, &UGameInstanceMain::QuickSave);
		PlayerInputComponent->BindAction("QuickLoad", IE_Pressed, GameInstance, &UGameInstanceMain::QuickLoad);
		PlayerInputComponent->BindAction("Climb", IE_Pressed, this, &AArcher::Climb);

		if (WeaponManagerComponent)
		{
			PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponManagerComponent,
			                                 &UWeaponManagerComponent::OnFireAction);
			PlayerInputComponent->BindAction("Fire", IE_Released, WeaponManagerComponent,
			                                 &UWeaponManagerComponent::OnFireReleasedAction);
			PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponManagerComponent,
			                                 &UWeaponManagerComponent::OnReloadAction);
			PlayerInputComponent->BindAction("ArmToggle", IE_Pressed, WeaponManagerComponent,
			                                 &UWeaponManagerComponent::OnToggleArmAction);
			PlayerInputComponent->BindAction("InterruptFire", IE_Pressed, WeaponManagerComponent,
			                                 &UWeaponManagerComponent::OnInterruptFireAction);
		}

		if (InventoryManagerComponent)
		{
			PlayerInputComponent->BindAction("DropFromInventory", IE_Pressed, InventoryManagerComponent,
			                                 &UInventoryManagerComponent::OnDropItemAction);
		}
	}
}

void AArcher::Climb()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Climb"));
	if (ClimbingMontage == nullptr || !GetMesh()->HasValidAnimationInstance())
	{
		return;
	}

	ClimbServer();
}


void AArcher::ClimbServer_Implementation()
{
	if (IsClimbing)
	{
		return;
	}


	// debug
	const FName TraceTag("Debug");
	GetWorld()->DebugDrawTraceTag = TraceTag;

	FCollisionQueryParams CollisionParams;
	CollisionParams.TraceTag = TraceTag;
	//

	FVector ActorLocation = GetActorLocation();

	FHitResult FrontTrace, HeightTrace;
	GetWorld()->LineTraceSingleByChannel(
		FrontTrace,
		ActorLocation,
		ActorLocation + (GetActorForwardVector() * 150),
		ECC_Visibility,
		CollisionParams
	);

	FVector Start = ActorLocation + FVector(0, 0, 500.f) + (GetActorForwardVector() * 75.f);
	FVector End = ActorLocation + (GetActorForwardVector() * 75.f);
	bool IsValidHeight = GetWorld()->LineTraceSingleByChannel(
		HeightTrace,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);

	if (!IsValidHeight)
	{
		return;
	}

	FVector WallTopHitLocation = HeightTrace.Location;
	FVector PelvisLocation = GetMesh()->GetSocketLocation("pelvisSocket");

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(WallTopHitLocation.Z - PelvisLocation.Z));
	if (WallTopHitLocation.Z - PelvisLocation.Z > 150) // too high	
	{
		return;
	}
	// all is ok

	IsClimbing = true;

	FVector WallNormal = FrontTrace.Normal;
	FVector WallHitLocation = FrontTrace.Location;

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();

	PlayClimbAnimationMulticast();

	FTimerHandle RestoreClimbingState;
	GetWorldTimerManager().SetTimer(RestoreClimbingState, [this]
	{
		IsClimbing = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Test");
	}, ClimbingMontage->SequenceLength, false);

	FVector ClimbLocation = FVector(
		WallHitLocation.X + WallNormal.X,
		WallHitLocation.Y + WallNormal.Y,
		WallTopHitLocation.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 19
	);
	GetCapsuleComponent()->SetRelativeLocation(ClimbLocation + (WallNormal * 32));

	FRotator ClimbRotation = UKismetMathLibrary::MakeRotFromX(WallNormal * -1);
	GetCapsuleComponent()->SetRelativeRotation(ClimbRotation);
}

void AArcher::PlayClimbAnimationMulticast_Implementation()
{
	float CompletedIn = PlayAnimMontage(
		ClimbingMontage,
		1.f,
		NAME_None
	);
}
