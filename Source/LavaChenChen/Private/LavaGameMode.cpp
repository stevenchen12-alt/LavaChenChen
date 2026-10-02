// LavaGameMode.cpp


#include "LavaGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.h"

ALavaGameMode::ALavaGameMode() {

}

void ALavaGameMode::BeginPlay() {
	Super::BeginPlay();

	KeysCollected = 0;
	LivesLeft = StartingLives;
	Score = 0;
	bGameOver = false;
	Message = TEXT("");

	// Timer fires every 1.0 second. Loops continuously (true)
	GetWorld()->GetTimerManager().SetTimer(LevelTimer, this, &ALavaGameMode::HandleTimeExpired, 1.0f, true);
}

void ALavaGameMode::EndPlay(const EEndPlayReason::Type Reason) {
	GetWorld()->GetTimerManager().ClearTimer(LevelTimer);

	Super::EndPlay(Reason);
}

void ALavaGameMode::ReportKeyCollected() {
	if (HasAllKeys()) {
		return;
	}

	KeysCollected++;

	Score += 200;

	if (KeysCollected > KeysRequired) {
		KeysCollected = KeysRequired;
	}
}

void ALavaGameMode::ReportLifeLost() {
	if (LivesLeft <= 0) {
		return;
	}

	LivesLeft--;
	Score -= 100;

	if (LivesLeft <= 0) {
		Message = TEXT("You died!");
		EndGame(false);
	}
}

void ALavaGameMode::ReportHatchReached() {
	if (HasAllKeys()) {
		EndGame(true);
	}

	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("You need all three keys to enter the hatch!"));
	}
}

float ALavaGameMode::GetTimeRemaining() const {
	return GetWorld()->GetTimerManager().GetTimerRemaining(LevelTimer);
}

void ALavaGameMode::EndGame(bool bWon) {
	if (bGameOver) {
		return;
	}

	GetWorld()->GetTimerManager().PauseTimer(LevelTimer);

	if (bWon) {
		Score += FMath::FloorToInt(GetTimeRemaining());
		Message = TEXT("You escaped the lava!");
	}

	bGameOver = true;

	if (ResultWidgetClass) {
		UResultWidget* ResultWidget = CreateWidget<UResultWidget>(GetWorld(), ResultWidgetClass);

		if (ResultWidget) {
			ResultWidget->SetupResultScreen(bWon, Score, Message);

			// Show the widget
			ResultWidget->AddToViewport();

			if (APlayerController* PC = GetWorld()->GetFirstPlayerController()) {
				PC->SetShowMouseCursor(true);

				// Swap input mode to UI only to be able to click on the widget
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(ResultWidget->TakeWidget());
				PC->SetInputMode(InputMode);
			}
		}
	}

	EndPlay(EEndPlayReason::LevelTransition);
}

void ALavaGameMode::HandleTimeExpired() {
	Message = TEXT("You ran out of time!");
	EndGame(false);
}

