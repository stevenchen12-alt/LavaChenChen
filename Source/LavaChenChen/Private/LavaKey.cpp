// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaKey.h"
#include "LavaCharacter.h"
#include "LavaGameMode.h"

// Sets default values
ALavaKey::ALavaKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALavaKey::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
}

// Called every frame
void ALavaKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Spin
	FRotator DeltaRotation(0.0f, SpinRate * DeltaTime, 0.0f);

	AddActorLocalRotation(DeltaRotation);

	// Bob
	float ElapsedSeconds = GetWorld()->GetTimeSeconds();
	float HeightOffset = FMath::Sin(ElapsedSeconds * BobSpeed) * BobHeight;

	FVector NewLocation = InitialLocation;
	NewLocation.Z += HeightOffset;
	SetActorLocation(NewLocation);
}

void ALavaKey::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep) {

	// Make sure the colliding actor exists and it not itself
	if (OtherActor && (OtherActor != this)) {

		// Make sure the colliding actor is the player character
		if (ALavaCharacter* PlayerCharacter = Cast<ALavaCharacter>(OtherActor)) {

			// Cast the current game mode to LavaGameMode
			if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(GetWorld()->GetAuthGameMode())) {				
				GameMode->ReportKeyCollected();
			}

			Destroy();
		}
	}
}