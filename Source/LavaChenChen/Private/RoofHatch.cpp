// RoofHatch.cpp

#include "RoofHatch.h"
#include "LavaCharacter.h"
#include "LavaGameMode.h"

// Sets default values
ARoofHatch::ARoofHatch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the roof hatch's mesh component
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	check(DoorFrameMesh != nullptr);

	SetRootComponent(DoorFrameMesh);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	check(DoorFrameMesh != nullptr);

	DoorMesh->SetupAttachment(DoorFrameMesh);

	// Create the collider
	CollisionRange = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionRange"));
	check(CollisionRange != nullptr);

	CollisionRange->SetupAttachment(DoorFrameMesh);
	CollisionRange->SetSphereRadius(32.f);
	CollisionRange->SetGenerateOverlapEvents(true);
	CollisionRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Remove engine defaults on collision channels
	CollisionRange->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Generate an overlap for any pawns
	CollisionRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ARoofHatch::BeginPlay()
{
	Super::BeginPlay();

	bIsOpening = false;
	bIsHatchOpened = false;
	
	// Ensure we don't bind the overlap event more than once
	CollisionRange->OnComponentBeginOverlap.RemoveAll(this);

	// Register the overlap event so it runs when an overlap happens
	CollisionRange->OnComponentBeginOverlap.AddDynamic(this, &ARoofHatch::HandleOverlap);

	DoorFrameMesh->SetVisibility(true);
	DoorMesh->SetVisibility(true);
	CollisionRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

			// Hatch has not been opened before
			if (!bIsHatchOpened) {
				OpenHatch();

				// Cast the current game mode to LavaGameMode
				if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(GetWorld()->GetAuthGameMode())) {
					GameMode->ReportHatchReached();
				}

				DoorFrameMesh->SetVisibility(false);
				DoorFrameMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				DoorMesh->SetVisibility(false);
				DoorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				CollisionRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ARoofHatch::OpenHatch() {
	bIsHatchOpened = true;
	bIsOpening = true;
}
