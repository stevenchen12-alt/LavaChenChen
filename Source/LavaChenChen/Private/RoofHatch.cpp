// RoofHatch.cpp

#include "Components/SphereComponent.h"
#include "RoofHatch.h"
#include "LavaCharacter.h"
#include "LavaGameMode.h"

// Sets default values
ARoofHatch::ARoofHatch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the key's mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	check(Mesh != nullptr);

	SetRootComponent(Mesh);

	// Create the collider
	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	check(PickupRange != nullptr);

	PickupRange->SetupAttachment(Mesh);
	PickupRange->SetSphereRadius(32.f);
	PickupRange->SetGenerateOverlapEvents(true);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Remove engine defaults on collision channels
	PickupRange->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Generate an overlap for any pawns
	PickupRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ARoofHatch::BeginPlay()
{
	Super::BeginPlay();
	
	// Ensure we don't bind the overlap event more than once
	PickupRange->OnComponentBeginOverlap.RemoveAll(this);

	// Register the overlap event so it runs when an overlap happens
	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &ARoofHatch::HandleOverlap);

	Mesh->SetVisibility(true);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called every frame
void ARoofHatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoofHatch::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep) {

	// Make sure the colliding actor exists and it is not itself
	if (OtherActor && (OtherActor != this)) {

		// Make sure the colliding actor is the player character
		if (ALavaCharacter* PlayerCharacter = Cast<ALavaCharacter>(OtherActor)) {

			// Cast the current game mode to LavaGameMode
			if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(GetWorld()->GetAuthGameMode())) {
				GameMode->ReportHatchReached();
			}

			Mesh->SetVisibility(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PickupRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

