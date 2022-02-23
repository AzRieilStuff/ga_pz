#include "AimAbility.h"

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
}

void UAimAbility::UpdateCameraPosition()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	const float Delta = TimerManager.GetTimerElapsed(AimingCameraTimer);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%f"), Delta));

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

	SourceCharacter->WeaponManagerComponent->OnChangeAimState.Broadcast(IsAim);

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

void UAimAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SourceCharacter->SetState(ECharacterStateFlags::Aiming);
	SetAimCamera(true);

	const float Duration = SourceCharacter->WeaponManagerComponent->CurrentWeapon->AimingDuration;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	                                 FString::FromInt(GetUniqueID()));
	GetWorld()->GetTimerManager().SetTimer(AimingCompleteTimer, [this]
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
		                                 FString::FromInt(GetUniqueID()));
		SourceCharacter->SetState(ECharacterStateFlags::AimReady);
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
	
	SourceCharacter->ClearState(ECharacterStateFlags::Aiming);
	SourceCharacter->ClearState(ECharacterStateFlags::AimReady);
}

void UAimAbility::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
