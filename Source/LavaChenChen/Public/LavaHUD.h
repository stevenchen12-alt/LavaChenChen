// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Lava.h"
#include "LavaHUD.generated.h"

class ALava;

UCLASS()
class LAVACHENCHEN_API ALavaHUD : public AHUD
{
	GENERATED_BODY()
	


protected: 
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LavaHUD")
	TSubclassOf<class UUserWidget> LavaHUDWidgetClass;
	
	UPROPERTY()
	UUserWidget* LavaHUDWidget;
	
	UPROPERTY()
	TObjectPtr<ALava> Lava;

	UFUNCTION(BlueprintPure, Category = "HUD Data")
	float GetLavaProgressPercent();
	
	UFUNCTION(BlueprintPure, Category = "HUD Data")
	FString GetTimeRemaining();
	
	UFUNCTION(BlueprintPure, Category = "HUD Data")
	int32 GetCurrentScore();
	
	UFUNCTION(BlueprintPure, Category = "HUD Data")
	int32 GetLivesRemaining();
	
};