// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"

#include "BaseInventoryItem.h"
#include "PickBoxComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Inventory/InventoryManagerComponent.h"

int32 ABaseItem::MaxPerStack;
bool ABaseItem::bStackable;

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickBoxComponent = CreateDefaultSubobject<UPickBoxComponent>("PickBoxComponent");
	SetRootComponent(PickBoxComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	//MeshComponent->SetMobility(EComponentMobility::Static);
	MeshComponent->SetIsReplicated(true);
	MeshComponent->SetupAttachment(RootComponent);

	bDestroyOnPickup = false;
	bReplicates = true;
	MaxPerStack = 1;
}


// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	PickBoxComponent->OnComponentHit.AddDynamic(this, &ABaseItem::OnHit);

	// allow to indicate drop only if item was pickable when spawned
	if (PickBoxComponent->IsSimulatingPhysics())
	{
		OnDropped();
	}
}


bool ABaseItem::CanPickupBy(AArcher* Character) const
{
	return Character->GetInventoryManagerComponent()->CanStoreItem(GenerateInventoryData()); // todo autouse? 
}

void ABaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UBaseInventoryItem* ABaseItem::GenerateInventoryData(UBaseInventoryItem* Target) const
{
	// Target must to be initialized in derived classes
	check(Target);

	Target->Name = GetName();
	Target->IconLabel = FString("");
	Target->Icon = InventoryIcon;
	Target->Amount = 1;

	Target->VisualActorClass = GetClass();


	return Target;
}

void ABaseItem::TryPickup(AArcher* Character)
{
	if (!bPickable || Character->GetLocalRole() <= ROLE_SimulatedProxy)
	{
		return;
	}

	if (Character->GetLocalRole() == ROLE_Authority)
	{
		if (Character->GetInventoryManagerComponent()->TryAddItem(this) && bDestroyOnPickup)
		{
			Destroy();
		}
	}
	else
	{
		bool LocalResult = Character->GetInventoryManagerComponent()->TryAddItem(this);
		// allow client to predict picking

		if (LocalResult)
		{
			PickBoxComponent->UnregisterComponent(); // prevent further interaction
			ServerPickup(Character);
		}
	}
}

bool ABaseItem::ServerPickup_Validate(AArcher* Character)
{
	return CanPickupBy(Character);
}

void ABaseItem::ServerPickup_Implementation(AArcher* Character)
{
	// keep server value synchronized with client
	Character->GetInventoryManagerComponent()->TryAddItem(this);

	if (bDestroyOnPickup)
	{
		Destroy(); // will be replicated by UE
	}
	else
	{
		MulticastPickup(Character); // item has no ownership, so use multicast for now
	}
}

void ABaseItem::MulticastPickup_Implementation(AArcher* Character)
{
	if (Character->IsLocallyControlled())
	{
		PickBoxComponent->RegisterComponent(); // restore active pickable state
	}
}

void ABaseItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	check(OtherActor);

	if (bPickable)
	{
		AArcher* Character = Cast<AArcher>(OtherActor);

		if (!Character)
		{
			return;
		}

		TryPickup(Character);
	}
}

void ABaseItem::OnStored()
{
	if (bDestroyOnPickup)
	{
		Destroy();
	}
}

void ABaseItem::OnDropped()
{
	PickBoxComponent->EnablePhysics();
}

void ABaseItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                      UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PickBoxComponent->DisablePhysics();
}
