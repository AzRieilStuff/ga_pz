// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "PZ_C_2/Weapon/BaseWeapon.h"
#include "PZ_C_2/Items/BaseItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"


ABaseCharacter::ABaseCharacter()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionProfileName(FName("Pawn"));
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
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::FromInt(RegenerationTicks));
		RestoreHealth(RegenerationRate);
		if (RegenerationTicks++ >= Ticks)
		{
			GetWorldTimerManager().ClearTimer(RegenerationTimerHandle);
		}
	};

	GetWorldTimerManager().SetTimer(RegenerationTimerHandle, Regeneration, RegenerationInterval, true, 0.f);
}

void ABaseCharacter::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// find weapon
	TArray<AActor*> Childs;
	GetAllChildActors(Childs, true);
	Childs.FindItemByClass<ABaseWeapon>(&Weapon);

	GetMovementComponent()->SetUpdatedComponent(MeshComp);
}

bool ABaseCharacter::PickItem(ABaseItem* Item)
{
	Item->UseItem(this);
	return true;
}
