#include "BaseBow.h"

#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Items/PickBoxComponent.h"

void ABaseBow::ShootProjectile()
{
	if (!ProjectileClass)
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

	SpawnParameters.Instigator = Character;

	//
	if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "shoot - Server");
	}
	else if(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetLocalRole() == ROLE_AutonomousProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "shoot - ROLE_AutonomousProxy");
	}
	//

	const FRotator ArrowRotation = Character->GetMesh()->GetSocketRotation(ArrowSocketName);
	const FVector SocketLocation = Character->GetMesh()->GetSocketLocation(ArrowSocketName);

	AArrow* Arrow = Cast<AArrow>(GetWorld()->SpawnActor(ProjectileClass,
	                                                    &SocketLocation,
	                                                    &ArrowRotation,
	                                                    SpawnParameters));

	if (Arrow == nullptr)
	{
		return;
	}

	Arrow->CollisionComponent->IgnoreActorWhenMoving(Character, true);
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
