// RoofHatch.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoofHatch.generated.h"

class UBoxComponent;

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
	TObjectPtr<UStaticMeshComponent> DoorFrameMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionRange;

	// --- Added ---
	bool bIsOpening = false;
	bool bIsHatchOpened = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// --- Added ---
	UFUNCTION()
	void OpenHatch();
	FRotator TargetRotation = FRotator(0.0f, 270.0f, 0.0f);
};
