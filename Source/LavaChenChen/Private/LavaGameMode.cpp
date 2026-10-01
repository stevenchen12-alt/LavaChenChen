// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaGameMode.h"

void ALavaGameMode::StartPlay() {
	Super::StartPlay();
	
	check(GEngine != nullptr);
}
