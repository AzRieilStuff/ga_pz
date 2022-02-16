// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManagerComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/PickBoxComponent.h"


UWeaponManagerComponent::UWeaponManagerComponent()
{
	bIsWeaponArmed = false;
}

void UWeaponManagerComponent::SetBowMeshVisibility(bool State) const
{
	Character->GetMesh()->ShowMaterialSection(0, 0, State, 0);
}

void UWeaponManagerComponent::SetCurrentWeapon(ABaseRangeWeapon* Weapon, ABaseRangeWeapon* PrevWeapon)
{
	if (Weapon == PrevWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, .3f, FColor::Red, "Trying to equip already equipped weapon");
		return;
	}

	if (PrevWeapon != nullptr) // unequip
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Unequip weapon");
		// #todo

		OnWeaponUnequipped.Broadcast(PrevWeapon);

		PrevWeapon->OwnerManagerComponent = nullptr;

		const FDetachmentTransformRules DetachOptions(EDetachmentRule::KeepWorld, true);
		PrevWeapon->DetachFromActor(DetachOptions);
	}
	//SetBowMeshVisibility(true);

	CurrentWeapon = Weapon;

	if (Weapon != nullptr) // equip
	{
		Weapon->GetPickBoxComponent()->DisablePhysics();
		Weapon->GetPickBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Weapon->SetOwner(GetOwner());
		Weapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                          BowBackSocket);

		Weapon->OwnerManagerComponent = this;

		// as it runs on server only, guarantees 1 run per actor
		OnWeaponEquipped.Broadcast(Weapon);

		UE_LOG(LogTemp, Warning, TEXT("Server: %s equip default weapon, ph %d"), *Character->GetName(),
		       Weapon->GetPickBoxComponent()->IsSimulatingPhysics() ? 1 : 0);
	}
}

void UWeaponManagerComponent::ServerEquipWeapon_Implementation(ABaseRangeWeapon* Weapon)
{
	EquipWeapon(Weapon);
}

void UWeaponManagerComponent::ServerUnequipWeapon_Implementation()
{
	UnequipWeapon();
}

void UWeaponManagerComponent::EquipWeapon(ABaseRangeWeapon* NewWeapon)
{
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		SetCurrentWeapon(NewWeapon, CurrentWeapon);
	}
	else
	{
		ServerEquipWeapon(CurrentWeapon);
	}
}

void UWeaponManagerComponent::EquipWeaponFromClass(TSubclassOf<ABaseRangeWeapon> WeaponClass)
{
	FActorSpawnParameters SpawnParameters;

	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = Character;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseRangeWeapon* Spawned = GetWorld()->SpawnActor<ABaseRangeWeapon>(
		WeaponClass, SpawnParameters
	);

	if (Spawned == nullptr)
	{
		return;
	}

	Spawned->GetPickBoxComponent()->DisablePhysics();
	Spawned->GetPickBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EquipWeapon(Spawned);
}

void UWeaponManagerComponent::UnequipWeapon()
{
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		SetCurrentWeapon(nullptr, CurrentWeapon);
	}
	else
	{
		ServerUnequipWeapon();
	}
}

void UWeaponManagerComponent::OnFireAction()
{
	if (CurrentWeapon == nullptr || !CurrentWeapon->CanFire() || !bIsWeaponArmed)
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(
		Character->GetMovementComponent());

	if (MovementComponent && MovementComponent->IsFalling())
	{
		return;
	}

	CurrentWeapon->FireAction();
}

void UWeaponManagerComponent::OnInterruptFireAction()
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	CurrentWeapon->InterruptFire();
}

void UWeaponManagerComponent::OnReloadAction()
{
}

void UWeaponManagerComponent::ServerReloadCurrentWeapon_Implementation()
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	CurrentWeapon->Reload();
}

void UWeaponManagerComponent::OnRep_CurrentWeapon(ABaseRangeWeapon* PrevWeapon)
{
	SetCurrentWeapon(CurrentWeapon, PrevWeapon);
	UE_LOG(LogTemp, Warning, TEXT("Weapon changed on client %s"), *Character->GetName());
}

void UWeaponManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AArcher>(GetOwner());
}

bool UWeaponManagerComponent::CanEquipWeapon(const ABaseRangeWeapon* NewWeapon) const
{
	// for now allow to equip any if no current weapon
	return CurrentWeapon == nullptr;
}

void UWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponManagerComponent, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UWeaponManagerComponent, Character, COND_InitialOnly);
}

void UWeaponManagerComponent::OnToggleArmAction()
{
	if (!IsWeaponEquipped())
	{
		return; // nothing to arm
	}

	// has conflicting state
	if (Character->HasState(ECharacterStateFlags::Firing) ||
		Character->HasState(ECharacterStateFlags::DisarmingBow) ||
		Character->HasState(ECharacterStateFlags::ArmingBow))
	{
		return;
	}

	if (GetNetMode() != NM_DedicatedServer)
	{
		bIsWeaponArmed ? DisarmWeapon() : ArmWeapon(); // run for local pawn
	}

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		bIsWeaponArmed ? MulticastDisarmWeapon() : MulticastArmWeapon();
	}
	else
	{
		bIsWeaponArmed ? ServerDisarmWeapon() : ServerArmWeapon();
	}
}

void UWeaponManagerComponent::ServerArmWeapon_Implementation()
{
	ArmWeapon();
	MulticastArmWeapon();
}

void UWeaponManagerComponent::ServerDisarmWeapon_Implementation()
{
	DisarmWeapon(); // run for server pawn
	MulticastDisarmWeapon();
}

void UWeaponManagerComponent::MulticastDisarmWeapon_Implementation()
{
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		DisarmWeapon();
	}
}

void UWeaponManagerComponent::MulticastArmWeapon_Implementation()
{
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		ArmWeapon();
	}
}

void UWeaponManagerComponent::DisarmWeapon()
{
	Character->SetState(ECharacterStateFlags::DisarmingBow);

	GetWorld()->GetTimerManager().SetTimer(DisarmTimer, this, &ThisClass::OnDisarmTimerEnds, Character->ArmingDuration,
	                                       false);
}

void UWeaponManagerComponent::OnDisarmTimerEnds()
{
	Character->ClearState(ECharacterStateFlags::DisarmingBow);
	bIsWeaponArmed = false;
}

void UWeaponManagerComponent::ArmWeapon()
{
	Character->SetState(ECharacterStateFlags::ArmingBow);

	GetWorld()->GetTimerManager().SetTimer(ArmTimer, this, &ThisClass::OnArmTimerEnds, Character->ArmingDuration,
	                                       false);
}

void UWeaponManagerComponent::OnArmTimerEnds()
{
	Character->ClearState(ECharacterStateFlags::ArmingBow);
	bIsWeaponArmed = true;
}

void UWeaponManagerComponent::OnDisarmWeaponPlaced()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, "OnDisarmWeaponPlaced");
	// reattach weapon from hand to back component
	CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                 BowBackSocket);
}

void UWeaponManagerComponent::OnArmWeaponPlaced()
{
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, "OnArmWeaponPlaced");
	CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                 BowArmSocket);
}
