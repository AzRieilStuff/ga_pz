// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	CollisionComponent->SetHiddenInGame(false);
	SetRootComponent(CollisionComponent);

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileImpact);
	}

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetIsReplicated(true);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MovementComponent->SetUpdatedComponent(RootComponent);
	MovementComponent->InitialSpeed = 1500.0f;
	MovementComponent->MaxSpeed = 1500.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->ProjectileGravityScale = 0.0f;
	MovementComponent->SetIsReplicated(true);

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;
}

void ABaseProjectile::BeginPlay()
{
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	Super::BeginPlay();
}

void ABaseProjectile::Destroyed()
{
	FVector SpawnLocation = GetActorLocation();
}

void ABaseProjectile::ApplyDamage(AActor* Actor, FVector Origin, const FHitResult& Hit)
{
	if (GetInstigator() == nullptr)
	{
		return;
	}

	DrawDebugBox(GetWorld(), Origin, FVector(2.f), FColor::Red, false, 3.f, 0, 0);

	UGameplayStatics::ApplyPointDamage(Actor, Damage, Origin, Hit, GetInstigator()->Controller, this,
	                                   DamageType);
}

void ABaseProjectile::PostActorCreated()
{
	Super::PostActorCreated();

}

void ABaseProjectile::MulticastPlayOnHitVisuals_Implementation()
{
	if (OnHitEffect == nullptr)
	{
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(this, OnHitEffect, GetActorLocation(), FRotator::ZeroRotator, true,
	                                         EPSCPoolMethod::AutoRelease);
}

void ABaseProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		ApplyDamage(OtherActor, Hit.Location, Hit);
	}

	if (bDestroyOnHit)
	{
		Destroy();
	}
	else
	{
		// disable collision
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		MulticastPlayOnHitVisuals();
	}
}
