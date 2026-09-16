#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

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

protected:
	void OnMoveAction(const FInputActionValue& InValue);
	void OnLookAction(const FInputActionValue& InValue);

private:
	UPROPERTY(VisibleAnywhere, Category = "MyAction|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "MyAction|Input")
	TObjectPtr<UInputAction> LookAction;
};
