// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"

#include "PZ_C_2/Characters/BaseCharacter.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
}


// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::UseItem(ABaseCharacter* Character)
{
	Destroy();
}

bool ABaseItem::CanUseBy(ABaseCharacter* Character)
{
	return true;
}

void ABaseItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if( bPickable ){
		ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);

		if( Character && CanUseBy(Character) ){
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Picking"));
		    Character->PickItem(this);
			FOnItemPicked.ExecuteIfBound(this, Character);
		}
	}
}

