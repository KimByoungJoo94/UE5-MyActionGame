#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::OpenMyLevel()
{
    UGameplayStatics::OpenLevel(this, TEXT("TutorialMap"));
}