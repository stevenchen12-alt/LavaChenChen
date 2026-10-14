// SprinklerTarget.cpp


#include "SprinklerTarget.h"
#include "LavaKey.h"

// Sets default values
ASprinklerTarget::ASprinklerTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the key's mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	check(Mesh != nullptr);

	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);

	Mesh->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ASprinklerTarget::BeginPlay()
{
	Super::BeginPlay();
	
	if (Mesh) {
		Mesh->OnComponentHit.AddDynamic(this, &ASprinklerTarget::HandleHit);
	}
}

// Called every frame
void ASprinklerTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASprinklerTarget::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (bHasBeenHit) {
		return;
	}

	//if (OtherActor && OtherActor->IsA(AExtinguisher::StaticClass())) {
	if (OtherActor) { // Remove this line and uncomment the one above when AExtinguisher is created
		bHasBeenHit = true;

		// Change the mesh or the material to look like it was hit
		if (Mesh && HitMaterial) {
			Mesh->SetMaterial(0, HitMaterial);
		}

		if (LinkedKey) {
			LinkedKey->UnlockKey();
		}

		Mesh->OnComponentHit.RemoveDynamic(this, &ASprinklerTarget::HandleHit);
	}
	
}
