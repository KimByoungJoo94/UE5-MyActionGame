#include "Components/MyStateComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UMyStateComponent::UMyStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMyStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMyStateComponent::ToggleMovementInput(bool bInEnabled, float InDuration /*= 0.1f*/)
{
	if (bInEnabled)
	{ 
		FLatentActionInfo LatentActionInfo;
		LatentActionInfo.CallbackTarget = this;
		LatentActionInfo.ExecutionFunction = TEXT("EnableMovementInput");
		LatentActionInfo.Linkage = 0;
		LatentActionInfo.UUID = 0;

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), InDuration, LatentActionInfo);
	}
	else
	{
		bIsMovementInputEnabled = false;
	}
}

void UMyStateComponent::EnableMovementInput()
{
	bIsMovementInputEnabled = true;
	ClearState();
}

void UMyStateComponent::ClearState()
{
	CurrentState = FGameplayTag::EmptyTag;
}

bool UMyStateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& InGameplayTagContainer) const
{
	return InGameplayTagContainer.HasTagExact(CurrentState);
}

bool UMyStateComponent::IsCurrentState(const FGameplayTag& InGameplayTag) const
{
	return CurrentState == InGameplayTag;
}

