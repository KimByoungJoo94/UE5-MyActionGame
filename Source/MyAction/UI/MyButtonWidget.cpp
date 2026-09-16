#include "MyButtonWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


TSharedRef<SWidget> UMyButtonWidget::RebuildWidget()
{
	if (WidgetTree->RootWidget == nullptr)
	{
		Button = WidgetTree->ConstructWidget<UButton>(
			UButton::StaticClass(),
			TEXT("Button"));

		TextBlock = WidgetTree->ConstructWidget<UTextBlock>(
			UTextBlock::StaticClass(),
			TEXT("TextBlock"));

		Button->AddChild(TextBlock);

		WidgetTree->RootWidget = Button;
	}

	return Super::RebuildWidget();
}

void UMyButtonWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (TextBlock)
	{
		TextBlock->SetText(ButtonText);
	}
}

void UMyButtonWidget::SetText(const FText& InText)
{
	ButtonText = InText;

	if (TextBlock)
	{
		TextBlock->SetText(ButtonText);
	}
}