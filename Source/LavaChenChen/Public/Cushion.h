#pragma once

#include "CoreMinimal.h"
#include "Throwable.h"
#include "Cushion.generated.h"

UCLASS()
class LAVACHENCHEN_API ACushion : public AThrowable
{
	GENERATED_BODY()

	
public:
	// Destroy cushion after 5 seconds
	void DestroyCushion();

	// Timer handle for the burning cushion
	FTimerHandle BurnTimerHandle;

	// Cushion last time
	UPROPERTY(EditDefaultsOnly, Category = "Cushion")
	float BurnTime = 5.0f;

protected:
	// Override HandleHit to listen for impacts with lava
	virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						   const FHitResult& Hit) override;

};