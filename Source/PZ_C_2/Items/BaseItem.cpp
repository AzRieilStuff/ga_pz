// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"

#include "PickBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickBoxComponent = CreateDefaultSubobject<UPickBoxComponent>("PickBoxComponent");
	SetRootComponent(PickBoxComponent);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	//MeshComponent->SetMobility(EComponentMobility::Static);
	MeshComponent->SetupAttachment(RootComponent);

	bDestroyOnPickup = false;
	bReplicates = true;
}


// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}


bool ABaseItem::CanPickupBy(AArcher* Character) const
{
	return true;
}

void ABaseItem::Pickup(AArcher* Character)
{
	ServerPickup(Character);

	PickBoxComponent->UnregisterComponent();

	if (bDestroyOnPickup)
	{
		SetHidden(true); // 'destroy' locally but keep valid for rpc calls
	}
}

void ABaseItem::ServerPickup_Implementation(AArcher* Character)
{
	MulticastPickup(Character);
}

void ABaseItem::MulticastPickup_Implementation(AArcher* Character)
{
	//FString Debug =
	//FString::Printf(TEXT("Multicast on %s"), *UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetName());
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Debug);

	if (PickBoxComponent && !Character->IsLocallyControlled())
	{
		PickBoxComponent->UnregisterComponent(); // #todo
	}

	if (bDestroyOnPickup && IsValid(this))
	{
		Destroy(); // Destroy for all
	}
	
	Character->OnItemPicked.Broadcast(this);
}

void ABaseItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bPickable)
	{
		AArcher* Character = Cast<AArcher>(OtherActor);

		if (Character && CanPickupBy(Character))
		{
			Pickup(Character);
			FOnItemPicked.ExecuteIfBound(this, Character);
		}
	}
}
