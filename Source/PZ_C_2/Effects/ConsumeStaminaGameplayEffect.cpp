#include "ConsumeStaminaGameplayEffect.h"

#include "PZ_C_2/Attributes/CharacterAttributeSet.h"

UConsumeStaminaGameplayEffect::UConsumeStaminaGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	//Period = 1.f; // per second

	// duration
	//FScalableFloat DurationScalableFloat;
	//DurationScalableFloat.Value = 10.f;
	//DurationMagnitude = FGameplayEffectModifierMagnitude(DurationScalableFloat);

	// stamina consume effect
	FGameplayModifierInfo StaminaModifierInfo;
	StaminaModifierInfo.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
	StaminaModifierInfo.ModifierOp = EGameplayModOp::Additive;

	// stamina consume amount
	FScalableFloat StaminaScalableFloat;
	StaminaScalableFloat.Value = -10.f; // todo
	StaminaModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(StaminaScalableFloat);
	StaminaModifierInfo.ModifierMagnitude = StaminaScalableFloat;

	Modifiers.Add(StaminaModifierInfo);
}
