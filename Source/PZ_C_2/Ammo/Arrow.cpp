// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

#include "Components/BoxComponent.h"
#include "PZ_C_2/Characters/Archer.h"


// Sets default values
AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = false;
	bDestroyOnHit = false;
	bReplicates = true;

	// Init components
	CollisionComponent->SetBoxExtent(FVector(24.f, 1.5f, 24.f));
	CollisionComponent->SetMobility(EComponentMobility::Movable);
	CollisionComponent->SetIsReplicated(true);
	SetRootComponent(CollisionComponent);

	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->SetRelativeLocation(FVector(-32.0, 0, 0.f));

	// Init assets from local lib
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(
		TEXT("/Game/Ammo/Arrow/SM_Arrow.SM_Arrow"), LOAD_None);

	if (MeshFinder.Succeeded())
	{
		MeshComponent->SetSkeletalMesh(MeshFinder.Object);
	}

	// config collision
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionComponent->SetCollisionProfileName("Projectile");
	CollisionComponent->SetGenerateOverlapEvents(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);

	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // others projectiles
}

void AArrow::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnProjectileImpact(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	AArcher* Character = Cast<AArcher>(OtherActor);
	if (Character != nullptr)
	{
		// sticking
		FName* Socket = Character->FindClosestSocket(GetActorLocation());
		if (Socket != nullptr)
		{
			SetOwner(Character);
			AttachToComponent(
				Character->GetMesh(),
				FAttachmentTransformRules::KeepWorldTransform,
				*Socket
			);
		}
		else
		{
			Destroy();
		}
	}
}

void AArrow::OnShoot()
{
	if (GetOwner() && GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, [this]
		{
			Destroy();
		}, Lifetime, false);
	}
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
}
