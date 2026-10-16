// Fill out your copyright notice in the Description page of Project Settings.

#include "Cushion.h"
#include "Lava.h" // Includes your ALava class definition
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

void ACushion::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::HandleHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	
	if (!OtherActor) return;
	
	if (OtherActor->IsA(ALava::StaticClass()))
	{
		if (Movement)
		{
			Movement->StopMovementImmediately();
			Movement->Deactivate();
		}
		GetWorld()->GetTimerManager().SetTimer(BurnTimerHandle, this, &ACushion::DestroyCushion,BurnTime, false);
	}
}

void ACushion::DestroyCushion()
{
	Destroy();
}