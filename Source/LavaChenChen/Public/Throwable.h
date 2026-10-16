// Throwable.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Throwable.generated.h" 

class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class LAVACHENCHEN_API AThrowable : public AActor
{
    GENERATED_BODY()

public:
    AThrowable();

    /** Called by the inventory when this item leaves the player's hand. */
    virtual void Throw(const FVector& Direction);

    /** While carried it must not fly, collide, or fall. */
    virtual void SetCarried(bool bCarried);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable")
    FVector ThrownScale = FVector(1.0f, 1.0f, 1.0f);

protected:
    virtual void BeginPlay() override;
    
    // The signature must match FComponentHitSignature exactly, and it must be a UFUNCTION.
    virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                           UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                           const FHitResult& Hit);

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UProjectileMovementComponent> Movement;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0"))
    float ThrowSpeed = 1600.f;
    
    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0"))
    float ProjectileGravity = 1.0f;
};