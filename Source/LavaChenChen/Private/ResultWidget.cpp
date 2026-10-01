// ResultWidget.cpp

#include "ResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::NativeConstruct() {
	Super::NativeConstruct();

	// Connect the restart button to OnPlayAgainClicked
	if (RestartButton) {
		RestartButton->OnClicked.AddDynamic(this, &UResultWidget::RestartLevel);
	}
}

void UResultWidget::SetupResultScreen(bool bWon, int32 Score, FString& Message) {
	if (ResultTitleText) {
		if (bWon) {
			ResultTitleText->SetText(FText::FromString("You Win!"));
		}

		else {
			ResultTitleText->SetText(FText::FromString("You Lost!"));
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

