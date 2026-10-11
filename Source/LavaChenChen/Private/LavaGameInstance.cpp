// LavaGameInstance.cpp


#include "LavaGameInstance.h"

void ULavaGameInstance::ResetRun() {
	Score = 0;
	BuildingIndex = 0;
}

void ULavaGameInstance::NextLevel() {
	BuildingIndex++;

	if (BuildingIndex >= TotalBuildings) {
		ResetRun();
	}
}