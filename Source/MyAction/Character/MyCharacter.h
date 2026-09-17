#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/MyGameplayTags.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMyAttributeComponent;
class UInputMappingContext;
class UInputAction;
class UMyPlayHUDWidget;
class UMyStateComponent;
class UMyCombatComponent;
class UAnimMontage;

struct FInputActionValue;

UCLASS()
class MYACTION_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsMoving() const;
	FORCEINLINE bool IsSprinting() const { return bSprinting; }
	FORCEINLINE TObjectPtr<UMyStateComponent> GetStateComponent()  { return StateComponent; }

	bool CanToggleCombat() const;

	const FGameplayTag GetAttackPerform() const;
	bool CanPerformAttack(const FGameplayTag& InAttackGameplayTag) const;

protected:
	void OnMoveActionTriggered(const FInputActionValue& InValue);
	void OnLookActionTriggered(const FInputActionValue& InValue);
	void OnSprintRollingActionTriggered();
	void OnSprintRollingActionCompleted();
	void OnSprintRollingActionCanceled();
	void OnInteractActionStarted();
	void OnToggleCombatActionStarted();
	void OnAttackActionStarted();
	void OnAttackActionCanceled();
	void OnAttackActionTriggered();
	void OnHeavyAttackActionStarted();

	void StartSprint();
	void StopSprint();
	void DoRolling();
	void DoInteraction();
	void DoToggleCombat();
	void AutoToggleCombat();
	void DoAttack();
	void DoSpecialAttack();
	void DoHeavyAttack();
	void DoComboAttack(const FGameplayTag& InAttackGameplayTag);
	void ResetComboAttack();
	void AttackByGameplayTag(const FGameplayTag& InAttackGameplayTag);

private:
	UPROPERTY(VisibleAnywhere, Category = "MyAction|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Attribute", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMyAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Attribute", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMyStateComponent> StateComponent;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMyCombatComponent> CombatComponent;
	
protected:
	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> SprintRollingAction;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> ToggleCombatAction;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> HeavyAttackAction;

protected:
	UPROPERTY(EditAnywhere, Category = "MyAction|UI")
	TSubclassOf<UMyPlayHUDWidget> PlayHUDWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "MyAction|UI")
	TObjectPtr<UMyPlayHUDWidget> PlayHUDWidget;

	UPROPERTY(EditAnywhere, Category = "MyAction|Anim")
	TObjectPtr<UAnimMontage> RollingAnimMontage;

protected:
	UPROPERTY(EditAnywhere, Category = "MyAction|Sprint")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "MyAction|Sprint")
	float NormalSpeed = 750.0f;

	UPROPERTY(EditAnywhere, Category = "MyAction|Sprint")
	bool bSprinting = false;

protected:
	bool bComboSequenceRunning = false;
	bool bCanComboInput = false;	
	bool bSavedComboInput = false;
	int32 ComboCounter = 0;
	FTimerHandle ComboResetTimerHandle;
};
