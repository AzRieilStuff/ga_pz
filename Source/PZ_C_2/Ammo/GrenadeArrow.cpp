// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeArrow.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AGrenadeArrow::AGrenadeArrow()
{
	DamageRadius = 250.f;
	Damage = 5.f;
	//bDestroyOnHit = true;
}

void AGrenadeArrow::ApplyDamage(AActor* Actor, FVector Origin, const FHitResult& Hit)
{
	UGameplayStatics::ApplyRadialDamage(Actor, Damage, Hit.Location, DamageRadius, DamageType, TArray<AActor*>(),
	                                    this, GetInstigator()->Controller);

	DrawDebugSphere(GetWorld(), Hit.Location, DamageRadius, 4, FColor::Red, false, 2.f);

	if( !IsActorBeingDestroyed() && bDestroyOnHit)
	{
		Destroy();
	}
}
