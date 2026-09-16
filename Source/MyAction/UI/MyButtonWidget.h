#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyButtonWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class MYACTION_API UMyButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetText(const FText& InText);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonText;

	UPROPERTY(Transient)
	TObjectPtr<UButton> Button;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> TextBlock;
};