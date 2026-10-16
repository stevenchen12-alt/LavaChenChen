// Fill out your copyright notice in the Description page of Project Settings.


#include "Extinguisher.h"
#include "Lava.h"

void AExtinguisher::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::HandleHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
    
	if (!OtherActor) return;

	if (OtherActor->IsA(ALava::StaticClass()))
	{ 
		Destroy();
	}
}