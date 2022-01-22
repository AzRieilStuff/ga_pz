// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"

#include "BaseInventoryItem.h"
#include "PickBoxComponent.h"
#include "GameFramework/GameModeBase.h"
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
	if (bStoreable)
	{
		return Character->InventoryManagerComponent->CanPickupItem(this);
	}

	return false; // todo autouse? 
}

UBaseInventoryItem* ABaseItem::GenerateInventoryData(UBaseInventoryItem* Target) const
{
	if (Target == nullptr)
	{
		Target = NewObject<UBaseInventoryItem>();
	}

	Target->Name = GetName();
	Target->IconLabel = FString("");
	Target->Icon = InventoryIcon;
	Target->Amount = 1;

	return Target;
}

void ABaseItem::Pickup(AArcher* Character)
{
	if (Character->GetLocalRole() > ROLE_SimulatedProxy)
	{ 
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, Character->GetName());
	}
	
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
	if (bStoreable)
	{
		if (Character->IsLocallyControlled())
		{
			Character->InventoryManagerComponent->ServerStoreItem(this);
			SetHidden(true); // we cant destroy object yet as we need valid Actor for RPC
		}
	}
	else
	{
		if (bDestroyOnPickup)
		{
			MarkPendingKill(); // Destroy for all
		}
	}
}

void ABaseItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	check(OtherActor);

	if (bPickable)
	{
		AArcher* Character = Cast<AArcher>(OtherActor);

		if (CanPickupBy(Character))
		{
			Pickup(Character);
			//FOnItemPicked.ExecuteIfBound(this, Character);
		}
	}
}

void ABaseItem::OnStored()
{
	if (bDestroyOnPickup)
	{
		Destroy();
	}
}
