#include "PassiveHealthRegeneration.h"

#include "PZ_C_2/Attributes/CharacterAttributeSet.h"

UPassiveHealthRegeneration::UPassiveHealthRegeneration()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1.f;

	// Health regeneration
	FGameplayModifierInfo HealthModifierInfo;
	HealthModifierInfo.Attribute = UCharacterAttributeSet::GetHealthAttribute();
	HealthModifierInfo.ModifierOp = EGameplayModOp::Additive;

	// Health regeneration amount
	FAttributeBasedFloat HealthRegenerationRate;
	HealthRegenerationRate.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeBaseValue;
	HealthRegenerationRate.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(
		UCharacterAttributeSet::GetHealthRegenRateAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false
	);

	HealthModifierInfo.ModifierMagnitude = HealthRegenerationRate;

	// confirm
	Modifiers.Add(HealthModifierInfo);
}
