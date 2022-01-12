// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
		bReplicates = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	CollisionComp->InitSphereRadius(5.f);
	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComp->SetHiddenInGame(false);
	RootComponent = CollisionComp;

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileImpact);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(
		TEXT("/Game/Common/SM_Cube.SM_Cube"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetIsReplicated(true);

	if (DefaultMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(DefaultMesh.Object);
		MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
		MeshComp->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
	}

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	MovementComp->SetUpdatedComponent(RootComponent);
	MovementComp->InitialSpeed = 1500.0f;
	MovementComp->MaxSpeed = 1500.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->SetIsReplicated(true);

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectile::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	if (OnHitEffect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, OnHitEffect, spawnLocation, FRotator::ZeroRotator, true,
		                                         EPSCPoolMethod::AutoRelease);
	}
}

void ABaseProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this,
		                                   DamageType);
	}

	Destroy();
}
