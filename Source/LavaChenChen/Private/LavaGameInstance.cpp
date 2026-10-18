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

void ULavaGameInstance::SetBuildingIndex(int32 newIndex) {
	if (newIndex >= TotalBuildings || newIndex < 0) {
		return;
	}

	BuildingIndex = newIndex;
}