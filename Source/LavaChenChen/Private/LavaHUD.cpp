// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaHUD.h"
#include "Lava.h"
#include "LavaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"


void ALavaHUD::BeginPlay()
{
	Super::BeginPlay();
	
	Lava = Cast<ALava>(UGameplayStatics::GetActorOfClass(GetWorld(), ALava::StaticClass()));
	LavaHUDWidget = CreateWidget(GetWorld(), LavaHUDWidgetClass);
	LavaHUDWidget->AddToViewport();
}

float ALavaHUD::GetLavaProgressPercent()
{
	float MaxBuildingHeight = 3178.f;
	float Percent = FMath::Clamp(Lava->GetRiseHeight()/ MaxBuildingHeight, 0.f, 1.f);
	return Percent;
}

FString ALavaHUD::GetTimeRemaining()
{
	if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		float TimeRemaining = GameMode->GetTimeRemaining();
		int32 Minutes = FMath::FloorToInt(TimeRemaining / 60.f);
		int32 Seconds = FMath::Modulo(TimeRemaining, 60.f);
		return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	}
	return TEXT("00:00");
}

int32 ALavaHUD::GetCurrentScore()
{
	if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return GameMode->GetScore();
	}
	return 0;
}

int32 ALavaHUD::GetLivesRemaining()
{
	if (ALavaGameMode* GameMode = Cast<ALavaGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		return GameMode->GetLivesLeft();
	}
	return 0;
}
