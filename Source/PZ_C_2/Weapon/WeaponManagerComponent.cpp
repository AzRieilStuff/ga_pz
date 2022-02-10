// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManagerComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/PickBoxComponent.h"

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
		if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
		{
			//FString Debug = FString::Printf(TEXT("Set %s weapon for %s"), *Weapon->GetName(), *GetOwner()->GetName());
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Debug);
		}

		Weapon->GetPickBoxComponent()->DisablePhysics();
		Weapon->GetPickBoxComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Weapon->SetOwner(GetOwner());
		Weapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                          FName("BowSocket"));
		//Weapon->SetActorRelativeLocation(FVector::ZeroVector);

		//CurrentWeapon = Weapon;
		Weapon->OwnerManagerComponent = this;

		// as it runs on server only, guarantees 1 run per actor
		OnWeaponEquipped.Broadcast(Weapon);
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

UWeaponManagerComponent::UWeaponManagerComponent()
{
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

void UWeaponManagerComponent::InteractWeapon()
{
	if (CurrentWeapon == nullptr || !CurrentWeapon->CanFire())
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
}

void UWeaponManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AArcher>(GetOwner());
	//Character = Cast<AArcher>(GetOwner());

	if (Character->GetLocalRole() == ROLE_SimulatedProxy && CurrentWeapon == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("didnt replicated"));
	}

	//UnequipWeapon();
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
