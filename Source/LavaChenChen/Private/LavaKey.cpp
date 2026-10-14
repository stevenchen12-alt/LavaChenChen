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
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	check(KeyMesh != nullptr);

	SetRootComponent(KeyMesh);

	// Create the collider
	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	check(PickupRange != nullptr);

	PickupRange->SetupAttachment(KeyMesh);
	PickupRange->SetSphereRadius(200.f);
	PickupRange->SetGenerateOverlapEvents(true);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Remove engine defaults on collision channels
	PickupRange->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Generate an overlap for any pawns
	PickupRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CageMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CageMesh1"));
	check(CageMesh1 != nullptr);

	CageMesh1->SetupAttachment(KeyMesh);

	CageMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CageMesh2"));
	check(CageMesh2 != nullptr);

	CageMesh2->SetupAttachment(CageMesh1);

	CageMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CageMesh3"));
	check(CageMesh3 != nullptr);

	CageMesh3->SetupAttachment(CageMesh2);
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

	KeyMesh->SetVisibility(true);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (bIsKeyOriginallyLocked) {
		CageMesh1->SetVisibility(true);
		CageMesh1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CageMesh1->SetCollisionResponseToAllChannels(ECR_Block);

		CageMesh2->SetVisibility(true);
		CageMesh2->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CageMesh2->SetCollisionResponseToAllChannels(ECR_Block);

		CageMesh3->SetVisibility(true);
		CageMesh3->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CageMesh3->SetCollisionResponseToAllChannels(ECR_Block);
	}

	else {
		CageMesh1->SetVisibility(false);
		CageMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CageMesh2->SetVisibility(false);
		CageMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CageMesh3->SetVisibility(false);
		CageMesh3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	

	bIsKeyCollected = false;
	bIsKeyLocked = bIsKeyOriginallyLocked;
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

			// Key has not been collected before and key is not locked
			if (!bIsKeyCollected && !bIsKeyLocked) {
				bIsKeyCollected = true;

				// Cast the current game mode to LavaGameMode
				if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(GetWorld()->GetAuthGameMode())) {
					GameMode->ReportKeyCollected();
				}

				KeyMesh->SetVisibility(false);
				KeyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				PickupRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

			else if (bIsKeyLocked) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Key is locked!"));
			}
		}
	}
}

void ALavaKey::DebugKeyPressed() {
	KeyMesh->SetVisibility(false);
	KeyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CageMesh1->SetVisibility(false);
	CageMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CageMesh2->SetVisibility(false);
	CageMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CageMesh3->SetVisibility(false);
	CageMesh3->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void ALavaKey::UnlockKey() {
	bIsKeyLocked = false;

	if (bIsKeyOriginallyLocked) {

		// The cage the key is locked in disappears
		if (CageMesh1) {
			CageMesh1->SetVisibility(false);
			CageMesh1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (CageMesh2) {
			CageMesh2->SetVisibility(false);
			CageMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (CageMesh3) {
			CageMesh3->SetVisibility(false);
			CageMesh3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}