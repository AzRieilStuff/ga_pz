#include "AimAbility.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PZ_C_2/Characters/Archer.h"
#include "PZ_C_2/Effects/ConsumeStaminaGameplayEffect.h"

UAimAbility::UAimAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	CostGameplayEffectClass = UConsumeStaminaGameplayEffect::StaticClass();
	AimingCameraTransitionDuration = 1.f;

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Aiming"));
}

void UAimAbility::UpdateCameraPosition()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	const float Delta = TimerManager.GetTimerElapsed(AimingCameraTimer);

	CameraInterpTime += Delta;
	{
		FVector CameraArmOffset;
		float CameraArmLength;

		CalcCameraPosition(
			CameraArmOffset,
			CameraArmLength,
			Delta,
			(CameraInterpTime / AimingCameraTransitionDuration) * 10.f // scale speed depends on left distance, 
		);

		SourceCharacter->GetSpringArmComponent()->TargetArmLength = CameraArmLength;
		SourceCharacter->GetSpringArmComponent()->TargetOffset = CameraArmOffset;
	}

	if (CameraInterpTime > AimingCameraTransitionDuration)
	{
		TimerManager.ClearTimer(AimingCameraTimer);
		CameraInterpTime = 0;
	}
}

void UAimAbility::CalcCameraPosition(FVector& Offset, float& Distance, const float Delta,
                                     const float InterpSpeed) const
{
	const USpringArmComponent* Arm = SourceCharacter->GetSpringArmComponent();

	Offset = FMath::VInterpTo(
		Arm->TargetOffset,
		SourceCharacter->CameraOffsetCurrent,
		Delta,
		InterpSpeed
	);

	Distance = FMath::FInterpTo(
		Arm->TargetArmLength,
		SourceCharacter->CameraDistanceCurrent,
		Delta,
		InterpSpeed
	);
}

void UAimAbility::SetAimCamera(const bool IsAim)
{
	SourceCharacter->CameraDistanceCurrent = IsAim
		                                         ? SourceCharacter->CameraDistanceAiming
		                                         : SourceCharacter->CameraDistanceDefault;
	SourceCharacter->CameraOffsetCurrent = IsAim
		                                       ? SourceCharacter->CameraOffsetAiming
		                                       : SourceCharacter->CameraOffsetDefault;

	SourceCharacter->GetWeaponManagerComponent()->OnChangeAimState.Broadcast(IsAim);

	// start animation
	const FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	// if we interrupt aiming, reuse current timer, so if we aiming for .7s, return in default state for the same
	if (TimerManager.IsTimerActive(AimingCameraTimer))
	{
		CameraInterpTime = AimingCameraTransitionDuration - CameraInterpTime;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(AimingCameraTimer, this, &ThisClass::UpdateCameraPosition,
		                                       .025f, true);
	}
}


void UAimAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo,
                              FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
                              const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	SourceCharacter = Cast<AArcher>(ActorInfo->OwnerActor);
}

bool UAimAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                FGameplayTagContainer* OptionalRelevantTags)
{
	if (!Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

bool UAimAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayTagContainer* SourceTags,
                                     const FGameplayTagContainer* TargetTags,
                                     FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UAimAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetAimCamera(true);

	const float Duration = SourceCharacter->GetWeaponManagerComponent()->CurrentWeapon->AimingDuration;

	GetWorld()->GetTimerManager().SetTimer(AimingCompleteTimer, [this, TriggerEventData]
	{
		bIsAimReady = true;
	}, Duration, false);
}

void UAimAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                             bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	SetAimCamera(false);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(AimingCompleteTimer);

	bIsAimReady = false;
}

void UAimAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
