// LavaGameMode.cpp


#include "LavaGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.h"
#include "LavaKey.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

ALavaGameMode::ALavaGameMode() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ALavaGameMode::BeginPlay() {
	Super::BeginPlay();

	KeysCollected = 0;
	LivesLeft = StartingLives;
	Score = 0;
	bGameOver = false;
	Message = TEXT("");

	// Set timer for LevelSeconds. Calls HandleTimeExpired when time runs out
	GetWorld()->GetTimerManager().SetTimer(LevelTimer, this, &ALavaGameMode::HandleTimeExpired, 1, true);
}

void ALavaGameMode::EndPlay(const EEndPlayReason::Type Reason) {
	GetWorld()->GetTimerManager().ClearTimer(LevelTimer);

	Super::EndPlay(Reason);
}

void ALavaGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!bGameOver)
	{
		float TimeRemaining = GetTimeRemaining();

		FString TimerMessage = FString::Printf(TEXT("Time Remaining: %.1f seconds"), TimeRemaining);

		// TBD
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, TimerMessage);
	}
}

void ALavaGameMode::ReportKeyCollected() {
	if (HasAllKeys()) {
		return;
	}

	KeysCollected++;

	Score += 200;

	FString KeyCollectionMessage = FString::Printf(TEXT("Key %d/%d Collected"), KeysCollected, KeysRequired);

	// TBD
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, KeyCollectionMessage);

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

	FString LivesLeftMessage = FString::Printf(TEXT("%d Lives Left"), LivesLeft);

	// TBD
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, LivesLeftMessage);

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
	return LevelSeconds;
}

int32 ALavaGameMode::GetScore() 
{
	return Score;
}

int32 ALavaGameMode::GetLivesLeft() const
{
	return LivesLeft;
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

	//EndPlay(EEndPlayReason::LevelTransition);
}

void ALavaGameMode::HandleTimeExpired() {
	LevelSeconds -= 1;
	if (LevelSeconds <= 0)
	{
		Message = TEXT("You ran out of time!");
		EndGame(false);
	}
}

void ALavaGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) {
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (NewPlayer) {

		// Bind 'k' to OnDebugKeyPressed() through the player's input component
		if (UInputComponent* IC = NewPlayer->InputComponent) {
			IC->BindKey(EKeys::K, IE_Pressed, this, &ALavaGameMode::DebugGiveAllKeys);
		}
	}
}

void ALavaGameMode::DebugGiveAllKeys() {
	KeysCollected = KeysRequired;

	FString DebugMessage = FString::Printf(TEXT("%d/%d keys collected!"), KeysCollected, KeysRequired);

	// Find all keys in the level
	TArray<AActor*> AllKeys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALavaKey::StaticClass(), AllKeys);

	for (AActor* Actor : AllKeys) {
		if (ALavaKey* KeyActor = Cast<ALavaKey>(Actor)) {
			KeyActor->DebugKeyPressed();
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, DebugMessage);
}