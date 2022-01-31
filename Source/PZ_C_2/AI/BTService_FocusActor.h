// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FocusActor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PZ_C_2_API UBTService_FocusActor : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_FocusActor();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
