// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseItem.h"

#include "PZ_C_2/Characters/TPCharacter.h"

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

void ABaseItem::UseItem(ATPCharacter* Character)
{
	Destroy();
}

bool ABaseItem::CanUseBy(ATPCharacter* Character)
{
	return true;
}

void ABaseItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if( bPickable ){
		ATPCharacter* Character = Cast<ATPCharacter>(OtherActor);

		if( Character && CanUseBy(Character) ){
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Picking"));
		    //Character->PickItem(this);
			FOnItemPicked.ExecuteIfBound(this, Character);
		}
	}
}

