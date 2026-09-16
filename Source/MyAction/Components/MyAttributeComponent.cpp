#include "Components/MyAttributeComponent.h"


UMyAttributeComponent::UMyAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMyAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UMyAttributeComponent::HasEnounghStamina(float InStamina) const
{
	return BaseStamina >= InStamina;
}

void UMyAttributeComponent::IncreaseStamina(float InStamina)
{
	const float OldStamina = BaseStamina;
	const float NewStamina = FMath::Clamp(OldStamina + InStamina, MinStamina, MaxStamina);

	if (FMath::IsNearlyEqual(OldStamina, NewStamina) == false)
	{
		BaseStamina = NewStamina;
		OnAttributeChanged.Broadcast(EMyAttributeType::Stamina, FMyAttributeChangeParam(BaseStamina, NewStamina, MinStamina, MaxStamina));
	}
}

void UMyAttributeComponent::DecreaseStamina(float InStamina)
{
	const float OldStamina = BaseStamina;
	const float NewStamina = FMath::Clamp(OldStamina - InStamina, MinStamina, MaxStamina);

	if (FMath::IsNearlyEqual(OldStamina, NewStamina) == false)
	{
		BaseStamina = NewStamina;
		OnAttributeChanged.Broadcast(EMyAttributeType::Stamina, FMyAttributeChangeParam(BaseStamina, NewStamina, MinStamina, MaxStamina));
	}
}

void UMyAttributeComponent::ToggleRegenerateStamina(bool bInEnabled, float InStartDelay /*= 2.0f*/)
{
	if (UWorld* World = GetWorld())
	{
		FTimerManager& WorldTimerManager = World->GetTimerManager();

		if (bInEnabled)
		{
			if (WorldTimerManager.IsTimerActive(RegenerateStaminaTimerHandle) == false)
			{
				WorldTimerManager.SetTimer(RegenerateStaminaTimerHandle, this, &ThisClass::RegenerateStaminaTimer, 0.1f, true, InStartDelay);
			}
		}
		else
		{
			WorldTimerManager.ClearTimer(RegenerateStaminaTimerHandle);
		}
	}
}

void UMyAttributeComponent::RegenerateStaminaTimer()
{
	IncreaseStamina(StaminaRegenRate);

	if (BaseStamina >= MaxStamina)
	{
		ToggleRegenerateStamina(false);
	}
}
