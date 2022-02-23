#include "PassiveRegeneration.h"

#include "PZ_C_2/Attributes/CharacterAttributeSet.h"

UPassiveRegeneration::UPassiveRegeneration()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1.f;
	
	// Health regeneration
	FGameplayModifierInfo HealthModifierInfo;
	HealthModifierInfo.Attribute = UCharacterAttributeSet::GetHealthAttribute();
	HealthModifierInfo.ModifierOp = EGameplayModOp::Additive;

	// Health regeneration amount
	FScalableFloat HealthScalableFloat;
	HealthScalableFloat.Value = 1.f; // todo
	HealthModifierInfo.ModifierMagnitude = HealthScalableFloat;

	// confirm
	Modifiers.Add(HealthModifierInfo);

	// Stamina regeneration
	FGameplayModifierInfo StaminaModifierInfo;
	StaminaModifierInfo.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
	StaminaModifierInfo.ModifierOp = EGameplayModOp::Additive;

	// Stamina regeneration amount
	FScalableFloat StaminaScalableFloat;
	StaminaScalableFloat.Value = 5.f; // todo
	StaminaModifierInfo.ModifierMagnitude = StaminaScalableFloat;

	// confirm
	Modifiers.Add(StaminaModifierInfo);
}
