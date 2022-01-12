// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

#include "Components/BoxComponent.h"


// Sets default values
AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = false;
	//bReplicates = true;

	// Init components
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	CollisionComponent->SetBoxExtent(FVector(1.5f, 1.5f, 24.f));
	CollisionComponent->SetMobility(EComponentMobility::Movable);
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComponent->SetMobility(EComponentMobility::Movable);
	//MeshComponent->SetWorldScale3D(FVector(4.f, 5.f, 4.f));
	MeshComponent->SetRelativeLocation(FVector(-32.0, 0, 0.f));
	//MeshComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.f, -90.f, 0.f)));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");

	// Init assets from local lib
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(
		TEXT("/Game/Ammo/Arrow/SM_Arrow.SM_Arrow"), LOAD_None);

	if (MeshFinder.Succeeded())
	{
		MeshComponent->SetSkeletalMesh(MeshFinder.Object);
	}

	// config
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
}
