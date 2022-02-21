#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "ConsumeStaminaTask.generated.h"

UCLASS()
class UConsumeStaminaTask : public UAbilityTask
{
	GENERATED_BODY()
public:
	UConsumeStaminaTask();

	virtual void Activate() override;

};
