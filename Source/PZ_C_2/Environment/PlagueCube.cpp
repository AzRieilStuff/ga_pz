// Fill out your copyright notice in the Description page of Project Settings.


#include "PlagueCube.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "PZ_C_2/Characters/BaseCharacter.h"

APlagueCube::APlagueCube()
{
	Collider = CreateDefaultSubobject<UBoxComponent>("Root");
	Collider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

APlagueCube::~APlagueCube()
{
}

void APlagueCube::BeginPlay()
{
	AActor::BeginPlay();

	if (!PlagueMesh)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(SpawnHandler, [&]
	{
		FillWithMeshes(FMath::RandRange(0, 100));
	}, 5.f, true, 0.f);
	
	
	GetWorldTimerManager().SetTimer(StateChangeHandler, [&]
	{
		for (auto It = Cubes.CreateConstIterator(); It; ++It)
		{
			Cubes[It.Key()] = CubeState::ACTIVE;
		}
	}, 1.f, true, 1.f);

	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &APlagueCube::OnEnter);
	Collider->OnComponentEndOverlap.AddDynamic(this, &APlagueCube::OnExit);
}

void APlagueCube::OnEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Character = Cast<ABaseCharacter, AActor>(OtherActor);
	if (Character != nullptr)
	{
		Character->SetRegeneration(-3.f, MAX_int32);
	}
}

void APlagueCube::OnExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex)
{
	ABaseCharacter* Character = Cast<ABaseCharacter, AActor>(OtherActor);
	if (Character != nullptr)
	{
		Character->SetRegeneration(0, 0);
	}
}


void APlagueCube::FillWithMeshes(int MeshCount)
{
	int Spawn = MeshCount - Cubes.Num();
	if (Spawn > 0)
	{
		for (int i = 0; i < Spawn; i++)
		{
			Cubes.Add(SpawnMeshItem(), CubeState::PASSIVE);
		}
	}
	else
	{
		ClearMeshes(-Spawn);
	}

	SpawnMeshItem();
}

void APlagueCube::ClearMeshes(int MeshCount)
{
	TArray<AStaticMeshActor*> CubesArray;
	Cubes.GenerateKeyArray(CubesArray);

	for (int i = 0; i < MeshCount; i++)
	{
		int Idx = FMath::RandRange(0, Cubes.Num() - 1);
		AStaticMeshActor* ToRemove = CubesArray[Idx];
		ToRemove->Destroy();
		Cubes.Remove(ToRemove);
	}
}

AStaticMeshActor* APlagueCube::SpawnMeshItem()
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	FVector Position = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), Collider->GetScaledBoxExtent());
	AStaticMeshActor* Actor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Position,
	                                                                   FRotator::ZeroRotator, ActorSpawnParams);

	float Size = FMath::RandRange(.2f, .4f);
	Actor->GetStaticMeshComponent()->SetRelativeScale3D(FVector(Size));
	Actor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	Actor->GetStaticMeshComponent()->SetStaticMesh(PlagueMesh);

	return Actor;
}
