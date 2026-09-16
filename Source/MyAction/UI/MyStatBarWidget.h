#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStatBarWidget.generated.h"


class UProgressBar;

UCLASS()
class MYACTION_API UMyStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	void SetPercent(float InPercent);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;

	UPROPERTY(EditAnywhere, Category = "MyAction|StatBar")
	FLinearColor ProgressBarColor;
};
