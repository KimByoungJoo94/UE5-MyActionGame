#include "MyStartMenuWidget.h"
#include "Components/Button.h"
#include "MyAction/MyGameInstance.h"

void UMyStartMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    TutorialButton->OnClicked.AddDynamic(this, &ThisClass::OnTutorialButtonClicked);
}

void UMyStartMenuWidget::NativeDestruct()
{
    TutorialButton->OnClicked.RemoveDynamic(this, &ThisClass::OnTutorialButtonClicked);

    Super::NativeDestruct();
}

void UMyStartMenuWidget::OnTutorialButtonClicked()
{
    if (UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance()))
    {
        MyGameInstance->OpenMyLevel();
    }
}