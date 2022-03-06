// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageEffect.h"
#include "PZ_C_2/Attributes/CharacterAttributeSet.h"

UDamageEffect::UDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Damage;
	FGameplayAttribute HealthAttr;

	HealthAttr.SetUProperty(UCharacterAttributeSet::GetHealthAttribute().GetUProperty());
	Damage.Attribute = HealthAttr;

	Modifiers.Add(Damage);
}
