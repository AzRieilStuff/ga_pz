#include "BaseBow.h"

#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/PickBoxComponent.h"

ABaseProjectile* ABaseBow::SpawnProjectile()
{
	if (!ProjectileClass)
	{
		return nullptr;
	}

	AArcher* Character = Cast<AArcher>(GetOwner());

	if (Character == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "wat");
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = Character;

	FRotator ArrowRotation;
	FVector SocketLocation;

	ComputeProjectileTransform(Character, SocketLocation, ArrowRotation);
	
	AArrow* Arrow = Cast<AArrow>(GetWorld()->SpawnActor(ProjectileClass,
	                                                    &SocketLocation,
	                                                    &ArrowRotation,
	                                                    SpawnParameters));

	if (Arrow == nullptr)
	{
		return nullptr;
	}

	return Arrow;
}

void ABaseBow::ComputeProjectileTransform(const AArcher* Character, FVector& Location, FRotator& Rotation)
{
	Location = Character->GetMesh()->GetSocketLocation(ArrowSocketName);
	Rotation = Character->GetMesh()->GetSocketRotation(ArrowSocketName);
}

ABaseBow::ABaseBow()
{
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> BowFinder(TEXT("/Game/Weapons/Bow/SM_Bow.SM_Bow"));

	if (BowFinder.Succeeded())
	{
		MeshComponent->SetSkeletalMesh(BowFinder.Object);
	}

	FireRate = 1.5f;
}

void ABaseBow::MulticastPickup_Implementation(AArcher* Character)
{
	if (Character && Character->WeaponManagerComponent)
	{
		Character->WeaponManagerComponent->EquipWeapon(this);
	}

	// todo replace with weapon equipped multicast
	Super::MulticastPickup_Implementation(Character);
}
