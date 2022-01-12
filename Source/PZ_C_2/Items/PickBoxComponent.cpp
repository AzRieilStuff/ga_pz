// Fill out your copyright notice in the Description page of Project Settings.


#include "PickBoxComponent.h"

#include "BaseItem.h"
#include "PZ_C_2/Characters/Archer.h"

UPickBoxComponent::UPickBoxComponent()
{
	if (GetOwner() == nullptr)
	{
		return;
	}

	Item = Cast<ABaseItem>(GetOwner());
	if (Item == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Pickalbe component can be used only within actor inherits ABaseItem"));
	}

	SetGenerateOverlapEvents(true);

	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//bHiddenInGame = false;

	//OnComponentBeginOverlap.AddDynamic(this, &UPickBoxComponent::OnActorBeginOverlap);
}

void UPickBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);
}


/*
void UPickBoxComponent::OnActorBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AArcher* Character = Cast<AArcher>(OtherActor);

	if (Character && Item && Item->CanPickupBy(Character))
	{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, OtherActor->GetName());
		Item->Pickup(Character);
	}

	DestroyComponent();
}
*/
