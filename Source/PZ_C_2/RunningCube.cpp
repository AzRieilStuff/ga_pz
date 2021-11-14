// Fill out your copyright notice in the Description page of Project Settings.


#include "RunningCube.h"

// Sets default values
ARunningCube::ARunningCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	// allow pawn to go through this objet
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ARunningCube::BeginPlay()
{
	Super::BeginPlay();
	if( IdleStateMaterial ){
		Mesh->SetMaterial(0, IdleStateMaterial);
	}

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ARunningCube::Activate);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &ARunningCube::Deactivate);

	OnStateChangeMulticastDelegate.AddDynamic(this, &ARunningCube::OnOverlapStateChange);
}


void ARunningCube::Activate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnStateChangeMulticastDelegate.Broadcast(true);
}

void ARunningCube::Deactivate(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnStateChangeMulticastDelegate.Broadcast(false);
}

void ARunningCube::Jump()
{
	const FVector NewPos = GetTransform().TransformPosition(FVector(0,100,0));
	SetActorLocation(NewPos);
}

void ARunningCube::OnOverlapStateChange(const bool State)
{
	if( State )
	{
		FTimerHandle MoveHandler;
		GetWorldTimerManager().SetTimer(MoveHandler, [&]
		{
			Jump();
		}, 3.f, false);
	}
}

// Called every frame
void ARunningCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

