#include "PickupStation.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LavaCharacter.h"
#include "InventoryComponent.h"
#include "TimerManager.h"

APickupStation::APickupStation()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    Trigger->SetupAttachment(RootComponent);

    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    
    Trigger->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
}

void APickupStation::BeginPlay()
{
    Super::BeginPlay();

    if (Trigger)
    {
        Trigger->OnComponentBeginOverlap.AddDynamic(this, &APickupStation::HandleOverlap);
    }
}

void APickupStation::EndPlay(const EEndPlayReason::Type Reason)
{
    if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RestockTimer))
    {
        GetWorld()->GetTimerManager().ClearTimer(RestockTimer);
    }

    Super::EndPlay(Reason);
}

void APickupStation::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                   bool bFromSweep, const FHitResult& Sweep)
{
    if (!bAvailable) return;

    ALavaCharacter* PlayerCharacter = Cast<ALavaCharacter>(OtherActor);
    if (!PlayerCharacter) return;

    UInventoryComponent* InventoryComp = PlayerCharacter->FindComponentByClass<UInventoryComponent>();
    if (!InventoryComp) return;

    if (InventoryComp->AddItem(ItemType))
    {
        bAvailable = false;

        if (RestockSeconds > 0.0f)
        {
            GetWorld()->GetTimerManager().SetTimer(RestockTimer, this, &APickupStation::Restock, RestockSeconds, false);
        }
        else
        {
            Restock();
        }
    }
}

void APickupStation::Restock()
{
    bAvailable = true;
}