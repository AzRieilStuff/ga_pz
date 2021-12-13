// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacter.h"
#include "PZ_C_2/Weapon/BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ATPCharacter::ATPCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ATPCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Childs;
	GetAllChildActors(Childs, true);
	Childs.FindItemByClass<ABaseWeapon>(&Weapon);

	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}

void ATPCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float ATPCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	// FString msg = FString::Printf(TEXT("%s receive damage %f"), *GetFName().ToString(),
	//                               DamageTaken);
	//
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, msg);

	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);

	return damageApplied;
}


void ATPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATPCharacter, CurrentHealth);
}

void ATPCharacter::OnHealthUpdate()
{
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("IsLocallyControlled HP %f"), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("%s killed"), *GetFName().ToString());
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, deathMessage);
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(
			TEXT("%s now has %f HP ( ROLE_Authority )"), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, healthMessage);
	}
}

void ATPCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

// Called every frame
void ATPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
