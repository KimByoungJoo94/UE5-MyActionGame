#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class MYACTION_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION()
	void AnimNotify_ResetMovementInput();

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyAction|Reference")
	TObjectPtr<ACharacter> OwningCharacter;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyAction|Reference")
	TObjectPtr<UCharacterMovementComponent> OwningCharacterMovementComponent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyAction|MovementData")
	FVector Velocity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyAction|MovementData")
	float GroundSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyAction|MovementData")
	bool bShouldMove;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyAction|MovementData")
	bool bIsFalling;
};
