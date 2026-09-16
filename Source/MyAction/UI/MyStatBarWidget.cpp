#include "UI/MyStatBarWidget.h"
#include "Components/ProgressBar.h"

void UMyStatBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ProgressBar)
	{
		ProgressBar->SetFillColorAndOpacity(ProgressBarColor);
	}
}

void UMyStatBarWidget::SetPercent(float InPercent)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(InPercent);
	}
}