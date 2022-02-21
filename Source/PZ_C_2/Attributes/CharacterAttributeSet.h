#pragma once

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)		\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

UCLASS(Blueprintable)
class UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", Replicated=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", Replicated=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", Replicated=OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Speed)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", Replicated=OnRep_SprintSpeed)
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, SprintSpeed)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", Replicated=OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", Replicated=OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina)

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData OldValue);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData OldValue );

	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData OldValue);

	UFUNCTION()
	void OnRep_SprintSpeed(const FGameplayAttributeData OldValue);
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData OldValue);
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData OldValue);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
};
