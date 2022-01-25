#include "BaseBow.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/Core/PickBoxComponent.h"

ABaseProjectile* ABaseBow::SpawnProjectile(FVector AimLocation)
{
	Super::SpawnProjectile(AimLocation);

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

	ComputeProjectileTransform(Character, AimLocation, SocketLocation, ArrowRotation);

	AArrow* Arrow = Cast<AArrow>(GetWorld()->SpawnActor(ProjectileClass,
	                                                    &SocketLocation,
	                                                    &ArrowRotation,
	                                                    SpawnParameters));

	if (Arrow == nullptr)
	{
		return nullptr;
	}

	if (OwnerManagerComponent)
	{
		OwnerManagerComponent->Character->MoveIgnoreActorAdd(Arrow);
	}

	return Arrow;
}

void ABaseBow::ComputeProjectileTransform(const AArcher* Character, FVector AimLocation, FVector& Location,
                                          FRotator& Rotation)
{
	Location = Character->GetMesh()->GetSocketLocation(ArrowSocketName);
	Rotation = Character->GetMesh()->GetSocketRotation(ArrowSocketName);

	Super::ComputeProjectileTransform(Character, AimLocation, Location, Rotation);
}

ABaseBow::ABaseBow()
{
	const ConstructorHelpers::FObjectFinder<UStaticMesh> BowFinder(TEXT("/Game/Weapons/Bow/SM_Bow.SM_Bow"));

	if (BowFinder.Succeeded())
	{
		MeshComponent->SetStaticMesh(BowFinder.Object);
	}

	FireRate = 1.5f;
}
