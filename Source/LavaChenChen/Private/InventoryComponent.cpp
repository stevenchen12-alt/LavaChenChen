// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "LavaCharacter.h"
#include "Throwable.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

bool UInventoryComponent::AddItem(EItemSlot Slot)
{
	FInventorySlot* SlotData = Slots.Find(Slot);
	// If there is no data, null then !0 = 1
	if (!SlotData)
	{
		return false;
	}
	// Checks if Slot is full
	if (SlotData->Count >= SlotData->Capacity)
	{
		return false;
	}
	
	SlotData->Count++;
	if (Slot == EquippedSlot && !HeldActor)
	{
		RefreshHeldActor();
	}
	return true;
}

void UInventoryComponent::EquipSlot(EItemSlot Slot)
{
	if (EquippedSlot != Slot)
	{
		EquippedSlot = Slot;
		RefreshHeldActor();
	}
}

void UInventoryComponent::ThrowEquipped()
{
	if (!HeldActor) return;
	
	FInventorySlot* SlotData = Slots.Find(EquippedSlot);
	if (!SlotData || SlotData->Count <= 0) return;
	
	SlotData->Count--;
	
	ALavaCharacter* LavaCharacter = Cast<ALavaCharacter>(GetOwner());
	if (LavaCharacter)
	{
		FRotator ControlRot = LavaCharacter->GetControlRotation();
		FVector ThrowDir = ControlRot.Vector();
		
		ThrowDir.Z += 0.35f; 
		ThrowDir.Normalize();
		
		FVector FinalThrowVelocity = ThrowDir * HeldActor->ThrowSpeed;
		
		FVector PlayerVelocity = LavaCharacter->GetVelocity();
		
		FinalThrowVelocity += (PlayerVelocity * 0.5f);
		
		HeldActor->Throw(FinalThrowVelocity);
	} else
	{
		HeldActor->Throw(FVector::ForwardVector * HeldActor->ThrowSpeed);
	}
	
	HeldActor = nullptr;
	if (SlotData->Count > 0)
	{
		RefreshHeldActor();
	}
}

int32 UInventoryComponent::GetCount(EItemSlot Slot) const
{
	const FInventorySlot* SlotData = Slots.Find(Slot);
	if (!SlotData)
	{
		return 0;
	}
	return SlotData->Count;
}

EItemSlot UInventoryComponent::GetEquippedSlot() const
{
	return EquippedSlot;
}

void UInventoryComponent::RefreshHeldActor()
{
	if (HeldActor)
	{
		HeldActor->Destroy();
		HeldActor = nullptr;
	}
	
	ALavaCharacter* LavaCharacter = Cast<ALavaCharacter>(GetOwner());
	FInventorySlot* SlotData = Slots.Find(EquippedSlot);
	UWorld* World = GetWorld();
	if (!SlotData || SlotData->Count <= 0) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = LavaCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	HeldActor = World->SpawnActor<AThrowable>(SlotData->ItemClass, LavaCharacter->GetActorTransform(), SpawnParams);
	if (HeldActor)
	{
		HeldActor->SetCarried(true);
		USkeletalMeshComponent* MeshComp = LavaCharacter->GetMesh();
		if (MeshComp && MeshComp->DoesSocketExist(HandSocketName))
		{
			HeldActor->AttachToComponent(
				MeshComp,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
				HandSocketName
			);
		}
	}
}

