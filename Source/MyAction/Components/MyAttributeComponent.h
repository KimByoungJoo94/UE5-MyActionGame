#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyAttributeComponent.generated.h"

UENUM(BlueprintType)
enum class EMyAttributeType : uint8
{
	Stamina,
	Health,
};

struct FMyAttributeChangeParam
{
	FMyAttributeChangeParam();
	FMyAttributeChangeParam(float InNewValue, float InOldValue, float InMinValue, float InMaxValue)
		: NewValue(InNewValue), OldValue(InOldValue), MinValue(InMinValue), MaxValue(InMaxValue)
	{

	}

	float NewValue;
	float OldValue;
	float MinValue;
	float MaxValue;
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, EMyAttributeType InAttributeType, const FMyAttributeChangeParam& InChangeParam)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTION_API UMyAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	UMyAttributeComponent();

protected:	
	virtual void BeginPlay() override;

public:		
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE float GetBaseStamina() { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	
	bool HasEnounghStamina(float InStamina) const;
	void IncreaseStamina(float InStamina);
	void DecreaseStamina(float InStamina);
	void ToggleRegenerateStamina(bool bInEnabled, float InStartDelay = 2.0f);

	FOnAttributeChanged& GetOnAttributeChanged() { return OnAttributeChanged; }

private:
	void RegenerateStaminaTimer();
		
protected:
	UPROPERTY(EditAnywhere, Category = "MyAction|Stamina")
	float BaseStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = "MyAction|Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = "MyAction|Stamina")
	float MinStamina = 0.0f;

	UPROPERTY(EditAnywhere, Category = "MyAction|Stamina")
	float StaminaRegenRate = 1.0f;

private:
	FTimerHandle RegenerateStaminaTimerHandle;

	FOnAttributeChanged OnAttributeChanged;
};
