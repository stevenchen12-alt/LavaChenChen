// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LavaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LAVACHENCHEN_API ALavaGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
};
