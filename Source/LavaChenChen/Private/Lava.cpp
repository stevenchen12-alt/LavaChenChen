// Fill out your copyright notice in the Description page of Project Settings.


#include "Lava.h"

#include "LavaCharacter.h"
#include "LavaGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALava::ALava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Creates the StaticMeshComponent Surface
	Surface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Surface"));
	RootComponent = Surface;
	// Creates the UBoxComponent for the volume collision box
	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	// Attaches the collision box to the StaticMeshComponent
	Volume->SetupAttachment(RootComponent);
	// Only check for collisions with pawn (player)
	Volume->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ALava::BeginPlay()
{
	Super::BeginPlay();
	
	// Record StartZ as the actor's initial Z value;
	StartZ = GetActorLocation().Z;
	// Directs the Volume collision box to redirect to HandleOverlap function whenever this (Lava) overlaps (with the pawn)
	if (Volume)
	{
		Volume->OnComponentBeginOverlap.AddDynamic(this, &ALava::HandleOverlap);
	}
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z += RiseRate * DeltaTime;
	SetActorLocation(CurrentLocation);
}

float ALava::GetRiseHeight() const
{
	return GetActorLocation().Z - StartZ;
}

void ALava::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					   bool bFromSweep, const FHitResult& Sweep)
{
	if (OtherActor && OtherActor != this)
	{	
		// Check if collision is with our LavaCharacter
		if (ALavaCharacter* LavaCharacter = Cast<ALavaCharacter>(OtherActor))
		{
			if (!bIsProtected)
			{
				bIsProtected = true;
				GetWorldTimerManager().SetTimer(ProtectedTimerHandle, this, &ALava::RemoveProtected, ProtectedTime, false);
				LavaCharacter->RespawnAtSafeLocation();
			}
		} 
	}
}

void ALava::RemoveProtected()
{
	bIsProtected = false;
}