// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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

	MovementComponent->SetAutoActivate(false);
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseProjectile::Destroyed()
{
	FVector SpawnLocation = GetActorLocation();

	if (OnHitEffect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, OnHitEffect, SpawnLocation, FRotator::ZeroRotator, true,
		                                         EPSCPoolMethod::AutoRelease);
	}
}

void ABaseProjectile::EnableMovement()
{
	MovementComponent->Velocity = GetActorRotation().Vector() * MovementComponent->InitialSpeed;
	MovementComponent->Activate();
}

void ABaseProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this,
		                                   DamageType);
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
}
