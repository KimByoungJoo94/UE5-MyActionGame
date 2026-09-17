#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "MyStateComponent.generated.h"

struct FGameplayTagContainer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTION_API UMyStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMyStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetState(const FGameplayTag InNewState) { CurrentState = InNewState; }
	FORCEINLINE FGameplayTag GetState() const { return CurrentState; }
	FORCEINLINE bool IsMovementInputEnabled() const { return bIsMovementInputEnabled; }

	void ToggleMovementInput(bool bInEnabled, float InDuration = 0.1f);
	
	UFUNCTION()
	void EnableMovementInput();

	void ClearState();

	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& InGameplayTagContainer) const;
	bool IsCurrentState(const FGameplayTag& InGameplayTag) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyAction|State")
	FGameplayTag CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyAction|State")
	bool bIsMovementInputEnabled = true;
};
