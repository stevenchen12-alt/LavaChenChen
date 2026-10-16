// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Throwable.h"
#include "Extinguisher.generated.h"

/**
 * 
 */
UCLASS()
class LAVACHENCHEN_API AExtinguisher : public AThrowable
{
	GENERATED_BODY()
	
	
protected:
	UFUNCTION()
	virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
