// Fill out your copyright notice in the Description page of Project Settings.


#include "SpyCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"

void ASpyCamera::RememberPawn(APawn* Pawn)
{
	DetectedPawn = Pawn;
	OnCharacterFound.ExecuteIfBound();
}

void ASpyCamera::ForgetPawn()
{
	DetectedPawn = nullptr;
	OnCharacterLost.ExecuteIfBound();
}

void ASpyCamera::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("In"));

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendTime = 2.f;
	PC->SetViewTarget(this, TransitionParams);

	APawn* Target = Cast<APawn, AActor>(OtherActor);
	if (Target == nullptr)
	{
		return;
	}
	FTimerDelegate ActorDelegate;
	ActorDelegate.BindUObject(this, &ASpyCamera::RememberPawn, Target);

	if (DetectedPawn != nullptr) // Detected previously pawn entered again
	{
		// reset "forgetting"
		GetWorldTimerManager().ClearTimer(ForgetTimer);
	}
	else // pawn enters first time
	{
		GetWorldTimerManager().SetTimer(FoundTimer, ActorDelegate, FoundDelay, false);
	}
}

void ASpyCamera::BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Out"));
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FViewTargetTransitionParams TransitionParams;
	TransitionParams.BlendTime = 2.f;
	PC->SetViewTarget(PC->GetPawn(), TransitionParams);
	
	APawn* Target = Cast<APawn, AActor>(OtherActor);
	if (Target == nullptr)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(FoundTimer);
	GetWorldTimerManager().ClearTimer(ForgetTimer);

	if (DetectedPawn)
	{
		GetWorldTimerManager().SetTimer(ForgetTimer, this, &ASpyCamera::ForgetPawn, ForgetDelay, false);
	}
}

void ASpyCamera::BeginPlay()
{
	Super::BeginPlay();

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASpyCamera::BoxBeginOverlap);
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ASpyCamera::BoxEndOverlap);
}

ASpyCamera::ASpyCamera()
{
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("TriggerArea");
	BoxTrigger->SetBoxExtent(FVector(500.f));
	BoxTrigger->SetCollisionObjectType(ECC_WorldStatic);
	BoxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxTrigger->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
}
