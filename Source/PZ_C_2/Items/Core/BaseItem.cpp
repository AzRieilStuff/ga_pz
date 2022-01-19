// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"

#include "PickBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickBoxComponent = CreateDefaultSubobject<UPickBoxComponent>("PickBoxComponent");
	SetRootComponent(PickBoxComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//MeshComponent->SetMobility(EComponentMobility::Static);
	MeshComponent->SetupAttachment(RootComponent);

	bDestroyOnPickup = false;
	bReplicates = true;
	MaxPerStack = 1;
}


// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}


bool ABaseItem::ServerPickup_Validate(AArcher* Character)
{
	return CanPickupBy(Character);
}

bool ABaseItem::CanPickupBy(AArcher* Character) const
{
	if( bStoreable )
	{
		return Character->InventoryManagerComponent->CanPickupItem(this);
	}
	
	return false; // todo autouse? 
}

void ABaseItem::GenerateInventoryData(FInventoryItem& InventoryData) const
{
	InventoryData.ItemClass = GetClass();
	InventoryData.Name = GetName();
	InventoryData.IconLabel = FString("-");
	InventoryData.Icon = InventoryIcon;
}

void ABaseItem::Pickup(AArcher* Character)
{
	ServerPickup(Character);

	if (bDestroyOnPickup)
	{
		PickBoxComponent->UnregisterComponent();
		SetHidden(true); // 'destroy' locally but keep valid for rpc calls
	}
}

void ABaseItem::ServerPickup_Implementation(AArcher* Character)
{
	MulticastPickup(Character);
}

void ABaseItem::MulticastPickup_Implementation(AArcher* Character)
{
	if (PickBoxComponent)
	{
		PickBoxComponent->UnregisterComponent(); // todo drop ability
	}

	// store into inventory for current pawn
	if( bStoreable && Character->IsLocallyControlled() )
	{
		Character->InventoryManagerComponent->ServerStoreItem(this);
	}
	
	if (bDestroyOnPickup && IsValid(this))
	{
		Destroy(); // Destroy for all
	}
	
}

void ABaseItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bPickable)
	{
		AArcher* Character = Cast<AArcher>(OtherActor);

		if (IsValid(Character) && CanPickupBy(Character))
		{
			Pickup(Character);
			//FOnItemPicked.ExecuteIfBound(this, Character);
		}
	}
}

bool ABaseItem::UseItem(AArcher* Target)
{
	return false;
}
