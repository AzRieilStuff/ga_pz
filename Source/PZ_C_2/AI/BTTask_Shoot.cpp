// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "AIController.h"
#include "PZ_C_2/Characters/Archer.h"

UBTTask_Shoot::UBTTask_Shoot()
{
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if( OwnerComp.GetAIOwner() == nullptr )
	{
		return EBTNodeResult::Failed;
	}

	AArcher* Character = Cast<AArcher>(OwnerComp.GetAIOwner()->GetPawn());

	if( Character == nullptr )
	{
		return EBTNodeResult::Failed;
	}

	if( Character->HasState(ECharacterStateFlags::Firing) )
	{
		return EBTNodeResult::InProgress;
	}

	Character->WeaponManagerComponent->OnFireAction();

	return EBTNodeResult::Succeeded;
}
