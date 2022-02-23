#pragma once

#include "Abilities/GameplayAbility.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Effects/ConsumeStaminaGameplayEffect.h"
#include "AimAbility.generated.h"

UCLASS(Blueprintable)
class UAimAbility : public UGameplayAbility
{
	GENERATED_BODY()
private:
	UPROPERTY()
	AArcher* SourceCharacter;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TSubclassOf<UConsumeStaminaGameplayEffect> AimStretchingGameplayEffect;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	float AimingCameraTransitionDuration;

	FTimerHandle AimingCameraTimer;
	FTimerHandle AimingCompleteTimer;

	// cumulative value for camera animation time
	float CameraInterpTime;

	void CalcCameraPosition(FVector& Offset, float& Distance, const float Delta, const float InterpSpeed) const;
	void UpdateCameraPosition();


	// [local]
	void SetAimCamera(const bool IsAim);

	bool bIsAimReady = false;
public:
	UAimAbility();

	inline bool GetIsAimReady() const { return bIsAimReady; }

	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                         const FGameplayAbilityActivationInfo ActivationInfo,
	                         FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
	                         const FGameplayEventData* TriggerEventData) override;

	/** Checks the ability's cost, but does not apply it. */
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo,
	                           FGameplayTagContainer* OptionalRelevantTags) override;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilityActivationInfo ActivationInfo,
	                           bool bReplicateCancelAbility) override;
};
