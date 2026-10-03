// LavaKey.cpp

#include "LavaKey.h"
#include "LavaCharacter.h"
#include "LavaGameMode.h"

// Sets default values
ALavaKey::ALavaKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the key's mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	check(Mesh != nullptr);

	SetRootComponent(Mesh);

	// Create the collider
	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	check(PickupRange != nullptr);

	PickupRange->SetupAttachment(Mesh);
	PickupRange->SetSphereRadius(200.f);
	PickupRange->SetGenerateOverlapEvents(true);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Remove engine defaults on collision channels
	PickupRange->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Generate an overlap for any pawns
	PickupRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ALavaKey::BeginPlay()
{
	Super::BeginPlay();

	// Ensure we don't bind the overlap event more than once
	PickupRange->OnComponentBeginOverlap.RemoveAll(this);

	// Register the overlap event so it runs when an overlap happens
	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &ALavaKey::HandleOverlap);

	InitialLocation = GetActorLocation();

	Mesh->SetVisibility(true);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	bIsKeyCollected = false;
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

	// Make sure the colliding actor exists and it is not itself
	if (OtherActor && (OtherActor != this)) {

		// Make sure the colliding actor is the player character
		if (ALavaCharacter* PlayerCharacter = Cast<ALavaCharacter>(OtherActor)) {

			// Key has not been collected before
			if (!bIsKeyCollected) {
				bIsKeyCollected = true;

				// Cast the current game mode to LavaGameMode
				if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(GetWorld()->GetAuthGameMode())) {
					GameMode->ReportKeyCollected();
				}

				Mesh->SetVisibility(false);
				Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				PickupRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ALavaKey::DebugKeyPressed() {
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}