#include "UI/MyPlayHUDWidget.h"
#include "UI/MyStatBarWidget.h"
#include "Components/MyAttributeComponent.h"


void UMyPlayHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPlayerPawn = GetOwningPlayerPawn())
	{
		if (UMyAttributeComponent* AttributeComponent = OwningPlayerPawn->GetComponentByClass<UMyAttributeComponent>())
		{
			OnAttributeChangedHandle = AttributeComponent->GetOnAttributeChanged().AddUObject(this, &ThisClass::OnAttributeChanged);
		}
	}
}

void UMyPlayHUDWidget::NativeDestruct()
{
	if (APawn* OwningPlayerPawn = GetOwningPlayerPawn())
	{
		if (UMyAttributeComponent* AttributeComponent = OwningPlayerPawn->GetComponentByClass<UMyAttributeComponent>())
		{
			AttributeComponent->GetOnAttributeChanged().Remove(OnAttributeChangedHandle);
		}
	}

	Super::NativeDestruct();
}

void UMyPlayHUDWidget::OnAttributeChanged(EMyAttributeType InAttributeType, const FMyAttributeChangeParam& InChangeParam)
{ 
	const float Percent = InChangeParam.NewValue / InChangeParam.MaxValue;
  	StaminaBarWidget->SetPercent(Percent);
}