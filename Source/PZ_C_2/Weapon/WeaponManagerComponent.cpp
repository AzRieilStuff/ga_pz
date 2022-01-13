// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManagerComponent.h"

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
		check(Character);
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
}

void UWeaponManagerComponent::UnequipWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Unequip weapon");
	//SetBowMeshVisibility(false);
	// #todo
}

void UWeaponManagerComponent::InteractWeapon()
{
	if (Weapon == nullptr || !Weapon->CanFire())
	{
		return;
	}

	Weapon->Fire();
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
