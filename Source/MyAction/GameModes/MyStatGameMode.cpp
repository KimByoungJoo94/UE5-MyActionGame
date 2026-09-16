#include "MyStatGameMode.h"
#include "MyAction/UI/MyStartMenuWidget.h"

void AMyStatGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UMyStartMenuWidget* CreatedWidget = CreateWidget<UMyStartMenuWidget>(GetWorld(), StartMenuWidgetClass))
	{	
		CreatedWidget->AddToViewport();
		StartMenuWidget = CreatedWidget;
	}
}

void AMyStatGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(StartMenuWidget))
	{
		StartMenuWidget->RemoveFromParent();
		StartMenuWidget = nullptr;

	}
	Super::EndPlay(EndPlayReason);
}

