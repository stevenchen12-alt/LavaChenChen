// RoofHatch.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoofHatch.generated.h"

UCLASS()
class LAVACHENCHEN_API ARoofHatch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoofHatch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& Sweep);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> PickupRange;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
