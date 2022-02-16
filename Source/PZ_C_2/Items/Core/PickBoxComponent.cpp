// Fill out your copyright notice in the Description page of Project Settings.


#include "PickBoxComponent.h"

#include "BaseItem.h"
#include "Net/UnrealNetwork.h"
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
	SetCollisionObjectType(ECC_WorldDynamic);
}

void UPickBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(true);
}

void UPickBoxComponent::EnablePhysics()
{
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	//                                  FString::Printf(TEXT("Enable for %s"), *GetOwner()->GetName()));
	SetNotifyRigidBodyCollision(true); // "generate hit events"
	SetSimulatePhysics(true);
	SetEnableGravity(true);
	SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); // do not interact until begin dropped
}

void UPickBoxComponent::DisablePhysics()
{
	SetNotifyRigidBodyCollision(false); // "generate hit events"
	SetSimulatePhysics(false);
	SetEnableGravity(false);
	SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UPickBoxComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
