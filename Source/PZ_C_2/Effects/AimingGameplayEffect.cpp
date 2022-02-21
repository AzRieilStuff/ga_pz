#include "AimingGameplayEffect.h"

#include "PZ_C_2/Attributes/CharacterAttributeSet.h"

UAimingGameplayEffect::UAimingGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1.f; // per second

	FGameplayModifierInfo StaminaMod;
	StaminaMod.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
	StaminaMod.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat StaminaCalculation;
	StaminaCalculation.Value = -10.f; // todo
	StaminaMod.ModifierMagnitude = FGameplayEffectModifierMagnitude( StaminaCalculation );

	StaminaMod.ModifierMagnitude = StaminaCalculation;
	
	Modifiers.Add(StaminaMod);
}
