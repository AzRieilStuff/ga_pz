#include "BaseBow.h"

#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/PickBoxComponent.h"

void ABaseBow::Shoot()
{
	if (!ArrowClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.Owner = GetOwner();
	AArcher* Character = Cast<AArcher>(GetOwner());

	if (Character == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "wat");
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Shoot");

	const FRotator ArrowRotation =Character->GetMesh()->GetSocketRotation(ArrowSocketName);
	const FVector SocketLocation = Character->GetMesh()->GetSocketLocation(ArrowSocketName);

	AArrow* Arrow = Cast<AArrow>(GetWorld()->SpawnActor(AArrow::StaticClass(),
	                                                    &SocketLocation,
	                                                    &ArrowRotation,
	                                                    SpawnParameters));

	if( Arrow == nullptr )
	{
		return;
	}

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

void ABaseBow::PickupMulticast_Implementation(AArcher* Character)
{
	if (Character && Character->WeaponManagerComponent)
	{
		Character->WeaponManagerComponent->EquipWeapon(this);
	}

	// todo replace with weapon equipped multicast
	Super::PickupMulticast_Implementation(Character);
}
