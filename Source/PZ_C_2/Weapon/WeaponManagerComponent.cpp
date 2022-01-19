// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManagerComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "PZ_C_2/Characters/Archer.h"

void UWeaponManagerComponent::SetBowMeshVisibility(bool State) const
{
	Character->GetMesh()->ShowMaterialSection(0, 0, State, 0);
}

UWeaponManagerComponent::UWeaponManagerComponent()
{
	if (GetOwner())
	{
		Character = Cast<AArcher>(GetOwner());
	}
	else
	{
		return;
	}
}

void UWeaponManagerComponent::EquipWeapon(ABaseRangeWeapon* NewWeapon)
{
	//SetBowMeshVisibility(true);

	NewWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                             FName("BowSocket"));
	NewWeapon->SetActorRelativeLocation(FVector::ZeroVector);
	NewWeapon->SetOwner(Character);

	Weapon = NewWeapon;
	Weapon->OwnerManagerComponent = this;

	OnWeaponEquipped.Broadcast(NewWeapon);
}

ABaseRangeWeapon* UWeaponManagerComponent::UnequipWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Unequip weapon");
	//SetBowMeshVisibility(false);
	// #todo

	ABaseRangeWeapon* OldWeapon = Weapon;
	
	OnWeaponUnequipped.Broadcast(OldWeapon);

	Weapon->OwnerManagerComponent = nullptr;
	Weapon = nullptr;

	const FDetachmentTransformRules DetachOptions(EDetachmentRule::KeepWorld, true);
	OldWeapon->DetachFromActor(DetachOptions);

	return OldWeapon;
}

void UWeaponManagerComponent::InteractWeapon()
{
	if (Weapon == nullptr || !Weapon->CanFire())
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(
		Character->GetMovementComponent());

	if( MovementComponent && MovementComponent->IsFalling())
	{
		return;
	}

	Weapon->FireAction();
}

void UWeaponManagerComponent::ReloadWeapon()
{
	if (Weapon == nullptr)
	{
		return;
	}

	Weapon->Reload();
}

void UWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UWeaponManagerComponent, Weapon, COND_InitialOnly);
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AArcher>(GetOwner());
	check(Character);

	//UnequipWeapon();
}

bool UWeaponManagerComponent::CanEquipWeapon(const ABaseRangeWeapon* NewWeapon) const
{
	// for now allow to equip any if no current weapon
	return Weapon == nullptr;
}
