// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

AEnemyAIController::AEnemyAIController()
{
	//PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent"));

	UAISenseConfig_Sight* Sense = NewObject<UAISenseConfig_Sight>(UAISenseConfig_Sight::StaticClass());

	Sense->SightRadius = 2000.f;
	Sense->DetectionByAffiliation.bDetectFriendlies = true; // ?
	Sense->DetectionByAffiliation.bDetectNeutrals = true;

	GetAIPerceptionComponent()->ConfigureSense(*Sense);
	GetAIPerceptionComponent()->SetDominantSense(Sense->GetSenseImplementation());

	GetAIPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// check visibility between actors
	FHitResult Trace;
	if (UpdatedActors.Num() > 0)
	{
		AActor* First = UpdatedActors[0];

		GetWorld()->LineTraceSingleByChannel(
			Trace,
			GetPawn()->GetActorLocation(),
			First->GetActorLocation(),
			ECC_Visibility
		);
		if (Trace.bBlockingHit) // actor get out of sight FOV
		{
			GetBlackboardComponent()->ClearValue(FName("Player"));
		}
		else // actor get into sight FOV
		{
			GetBlackboardComponent()->SetValueAsObject(FName("Player"), First);
		}
	}
	else
	{
		GetBlackboardComponent()->ClearValue(FName("Player"));
	}
}
