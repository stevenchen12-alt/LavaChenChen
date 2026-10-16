// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AThrowable::AThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = Mesh;
	Movement->InitialSpeed = ThrowSpeed;
	Movement->MaxSpeed = ThrowSpeed;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = ProjectileGravity;
	Movement->bAutoActivate = false;
	
}

// Called when the game starts or when sp	awned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh)
	{
		Mesh->OnComponentHit.AddDynamic(this, &AThrowable::HandleHit);
	}
}

void AThrowable::Throw(const FVector& Direction)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetCarried(false);
	if (Movement)
	{
		Movement->SetActive(true);
		Movement->Velocity = Direction.GetSafeNormal() * ThrowSpeed;
	}
}

void AThrowable::SetCarried(bool bCarried)
{
	if (bCarried)
	{
		if (Movement)
		{
			Movement->StopMovementImmediately();
			Movement->SetActive(false);
		}
		if (Mesh)
		{
			Mesh->SetSimulatePhysics(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	} else
	{
		if (Mesh)
		{
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void AThrowable::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						   const FHitResult& Hit)
{

}

