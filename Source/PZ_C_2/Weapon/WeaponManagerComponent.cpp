// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PZ_C_2/Abilities/AimAbility.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/BaseInventoryItem.h"
#include "PZ_C_2/Items/Core/PickBoxComponent.h"


UWeaponManagerComponent::UWeaponManagerComponent()
{
	bIsWeaponArmed = false;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
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

void UWeaponManagerComponent::AutoEquipWeapon()
{
	if (IsWeaponEquipped())
	{
		return;
	}

	const UBaseInventoryItem* Weapon = Character->GetInventoryManagerComponent()->GetActiveItem(
		EInventorySlot::MainWeapon);
	if (Weapon == nullptr || !Weapon->VisualActorClass->IsChildOf(ABaseRangeWeapon::StaticClass()))
	{
		return;
	}

	const TSubclassOf<ABaseRangeWeapon> WeaponClass = *Weapon->VisualActorClass;
	EquipWeaponFromClass(WeaponClass);
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
	if (CurrentWeapon == nullptr || !CurrentWeapon->CanFire())
	{
		return;
	}

	if (Character->GetCharacterMovementComponent()->IsFalling())
	{
		return;
	}

	// we have unarmed valid weapon, arm
	if (!bIsWeaponArmed)
	{
		OnToggleArmAction();
		return;
	}

	FGameplayAbilitySpec* AimAbilitySpec = Character->GetAbilitySpecByKey(EAbility::Aim);

	if (AimAbilitySpec && AimAbilitySpec->Handle.IsValid())
	{
		Character->GetAbilitySystemComponent()->TryActivateAbility(AimAbilitySpec->Handle);
	}
}

void UWeaponManagerComponent::OnFireReleasedAction()
{
	const FGameplayAbilitySpec* AimAbilitySpec = Character->GetAbilitySpecByKey(EAbility::Aim);

	if (AimAbilitySpec == nullptr || !AimAbilitySpec->IsActive())
	{
		return;
	}

	UAimAbility* AimAbility = Cast<UAimAbility, UGameplayAbility>(*AimAbilitySpec->GetAbilityInstances().GetData());

	if (AimAbility == nullptr)
	{
		return;
	}

	if (AimAbility->GetIsAimReady())
	{
		CurrentWeapon->Fire();
		Character->GetAbilitySystemComponent()->CancelAbilityHandle(AimAbilitySpec->Handle);
	}
	else
	{
		Character->GetAbilitySystemComponent()->CancelAbilityHandle(AimAbilitySpec->Handle);
	}
}

void UWeaponManagerComponent::OnInterruptFireAction()
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	FGameplayAbilitySpec* AimAbility = Character->GetAbilitySpecByKey(EAbility::Aim);
	if (AimAbility != nullptr && AimAbility->Handle.IsValid())
	{
		Character->GetAbilitySystemComponent()->CancelAbilityHandle(AimAbility->Handle);
	}

	//CurrentWeapon->InterruptFire();
}

bool UWeaponManagerComponent::HasAmmo() const
{
	UBaseInventoryItem* AmmoPack = Character->GetInventoryManagerComponent()->GetActiveItem(
		EInventorySlot::MainWeaponAmmo);

	return AmmoPack != nullptr && AmmoPack->GetAmount() > 0;
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

void UWeaponManagerComponent::OnRegister()
{
	Super::OnRegister();

	Character = Cast<AArcher>(GetOwner());
	check(Character);
}

void UWeaponManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	Character->GetInventoryManagerComponent()
	         ->OnInventoryStateChange.AddUObject(this, &ThisClass::AutoEquipWeapon);
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

void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWeaponManagerComponent::IsAiming()
{
	return IsWeaponEquipped() && Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag("Character.Aiming")
	);
}

void UWeaponManagerComponent::OnToggleArmAction()
{
	if (!IsWeaponEquipped())
	{
		return; // nothing to arm
	}

	// has conflicting state
	if (
		Character->GetWeaponManagerComponent()->IsAiming() ||
		Character->HasState(ECharacterStateFlags::DisarmingBow) ||
		Character->HasState(ECharacterStateFlags::ArmingBow)
	)
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
