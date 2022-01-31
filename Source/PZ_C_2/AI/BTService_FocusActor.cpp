// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FocusActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PZ_C_2/Characters/Archer.h"

UBTService_FocusActor::UBTService_FocusActor()
{
	NodeName = FString("Focus Archer Character");
}

void UBTService_FocusActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const FName FocusBBKey = GetSelectedBlackboardKey();
	AArcher* Character = Cast<AArcher>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FocusBBKey));

	if (Character == nullptr)
	{
		return;
	}

	//FVector Rotation = OwnerComp.GetOwner()->GetActorLocation() - Character->GetActorLocation();
	//Rotation.Normalize();
	OwnerComp.GetAIOwner()->SetFocus(Character);
}
