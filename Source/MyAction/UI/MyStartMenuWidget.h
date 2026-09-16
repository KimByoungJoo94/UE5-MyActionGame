#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStartMenuWidget.generated.h"

class UButton;

UCLASS()
class MYACTION_API UMyStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnTutorialButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TutorialButton;
};
