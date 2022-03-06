#include "PassiveStaminaRegeneration.h"

#include "PZ_C_2/Attributes/CharacterAttributeSet.h"

UPassiveStaminaRegeneration::UPassiveStaminaRegeneration()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1.f;

	// Stamina regeneration
	FGameplayModifierInfo StaminaModifierInfo;
	StaminaModifierInfo.Attribute = UCharacterAttributeSet::GetStaminaAttribute();
	StaminaModifierInfo.ModifierOp = EGameplayModOp::Additive;

	// Stamina regeneration amount
	FAttributeBasedFloat StaminaRegenerationRate;
	StaminaRegenerationRate.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeBaseValue;
	StaminaRegenerationRate.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(
		UCharacterAttributeSet::GetStaminaRegenRateAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false // recalculation on period tick
	);

	StaminaModifierInfo.ModifierMagnitude = StaminaRegenerationRate;

	// confirm
	Modifiers.Add(StaminaModifierInfo);

	// tags
	OngoingTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Aiming"));
}
