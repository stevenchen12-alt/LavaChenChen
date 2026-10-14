// SprinklerTarget.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprinklerTarget.generated.h"

class ALavaKey;

UCLASS()
class LAVACHENCHEN_API ASprinklerTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprinklerTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --- Added ---
	UFUNCTION()
	void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UMaterialInterface> NotHitMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UMaterialInterface> HitMaterial;

	UPROPERTY(EditInstanceOnly, Category = "Target")
	ALavaKey* LinkedKey;

	// Static meshes for hit and not hit targets
	UPROPERTY()

	bool bHasBeenHit = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
