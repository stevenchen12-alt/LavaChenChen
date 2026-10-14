// ResultWidget.cpp

#include "ResultWidget.h"
#include "LavaGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::NativeConstruct() {
	Super::NativeConstruct();

	// Connect the restart button to RestartLevel()
	if (RestartButton) {
		RestartButton->OnClicked.AddDynamic(this, &UResultWidget::RestartLevel);
	}

	// Connect the next button to NextLevel()
	if (NextButton) {
		NextButton->OnClicked.AddDynamic(this, &UResultWidget::NextLevel);
	}
}

void UResultWidget::SetupResultScreen(bool bWon, int32 Score, FString& Message) {
	if (ResultTitleText) {
		if (bWon) {
			ResultTitleText->SetText(FText::FromString("You Win!"));

			if (RestartButton) {
				RestartButton->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (NextButton) {
				NextButton->SetVisibility(ESlateVisibility::Visible);
			}
		}

		else {
			ResultTitleText->SetText(FText::FromString("You Lost!"));

			if (RestartButton) {
				RestartButton->SetVisibility(ESlateVisibility::Visible);
			}

			if (NextButton) {
				NextButton->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}

	if (ResultText) {
		Message += TEXT("\n\nScore: ");
		Message += FString::FromInt(Score);

		ResultText->SetText(FText::FromString(Message));
	}
}

void UResultWidget::RestartLevel() {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0)) {
		PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

}

void UResultWidget::NextLevel() {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0)) {
		PC->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	FName NextLevelName = NAME_None;

	ULavaGameInstance* LavaGameInstance = Cast<ULavaGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (LavaGameInstance) {
		switch (LavaGameInstance->BuildingIndex) {
			case 0:
				NextLevelName = TEXT("BTHS");
				break;
			
			case 1:
				NextLevelName = TEXT("ArchesNationalParkMap");
				break;
		}
			
		if (!NextLevelName.IsNone()) {
			UGameplayStatics::OpenLevel(this, NextLevelName, false);
		}

		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Cannot open the next level!"));
		}
	}
}