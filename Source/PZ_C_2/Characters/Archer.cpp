// Fill out your copyright notice in the Description page of Project Settings.


#include "Archer.h"

#include "Components/CapsuleComponent.h"
#include "PZ_C_2/Weapon/BaseRangeWeapon.h"
#include "PZ_C_2/Inventory/Inventory.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PZ_C_2/Framework/GameInstanceMain.h"

// Sets default values
AArcher::AArcher()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>("WeaponManager");
	WeaponManagerComponent->SetIsReplicated(true);

	InventoryComponent = CreateDefaultSubobject<UInventory>("InventoryComponent");
	InventoryComponent->SetIsReplicated(true); //todo why

	GetMesh()->SetIsReplicated(true); // replicate bone rotation
	
	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	GetCharacterMovement()->JumpZVelocity = 800.f;
}

// Called when the game starts or when spawned
void AArcher::BeginPlay()
{
	Super::BeginPlay();

	// Init default weapon
	if (HasAuthority() && DefaultWeapon)
	{
		SetWeaponFromClass(DefaultWeapon);
	}
}

void AArcher::SetCurrentHealth(float healthValue)
{
	// Update server value
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate(); // if server changes value, repNotify wont call
	}
}

float AArcher::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
                          AActor* DamageCauser)
{
	Super::TakeDamage(DamageTaken, DamageEvent, EventInstigator, DamageCauser);

	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);

	return damageApplied;
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
			                                 &UWeaponManagerComponent::InteractWeapon);
			PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponManagerComponent,
			                                 &UWeaponManagerComponent::ReloadWeapon);
		}
	}
}

void AArcher::SetWeaponFromActor(ABaseRangeWeapon* NewWeapon)
{
	if (GetWeapon() != nullptr)
	{
		WeaponComponent->DestroyChildActor();
	}
	NewWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                             FName("left_cannon"));
	NewWeapon->SetOwner(this);
}

void AArcher::SetWeaponFromClass(TSubclassOf<ABaseRangeWeapon> NewWeapon)
{
	if (GetWeapon() != nullptr)
	{
		WeaponComponent->DestroyChildActor();
	}

	WeaponComponent->SetChildActorClass(NewWeapon);
	WeaponComponent->CreateChildActor();

	AActor* Created = WeaponComponent->GetChildActor();
	if (IsValid(Created))
	{
		Created->AttachToComponent(this->GetMesh(),
		                           FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		                           FName("left_cannon"));
		Created->SetOwner(this);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, .3f, FColor::Red, "Failed to create weapon");
	}
}

ABaseRangeWeapon* AArcher::GetWeapon() const
{
	if (WeaponComponent != nullptr)
	{
		return Cast<ABaseRangeWeapon>(WeaponComponent->GetChildActor());
	}

	return nullptr;
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

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->StopMovementImmediately();

	PlayClimbAnimationMulticast();

	FTimerHandle RestoreClimbingState;
	GetWorldTimerManager().SetTimer(RestoreClimbingState, [this]
	{
		IsClimbing = false;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
