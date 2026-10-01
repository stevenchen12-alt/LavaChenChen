// ResultWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class LAVACHENCHEN_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupResultScreen(bool bWon, int32 Score, FString& Message);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void RestartLevel();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultTitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultText;
};
