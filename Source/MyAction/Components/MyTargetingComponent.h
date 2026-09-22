#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyTargetingComponent.generated.h"

class UCameraComponent;

UENUM(BlueprintType)
enum class EMyTargetingSwitchDirection : uint8
{
	None,
	Left,
	Right,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTION_API UMyTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	UMyTargetingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool IsLockOn() const { return bIsLockOn; }

	void ToggleLockOnff();
	void SwitchLockTargetActor(EMyTargetingSwitchDirection InDirection);
	
protected:
	void FindTargetArray(OUT TArray<TObjectPtr<AActor>>& OutTargetActorArray) const;
	TObjectPtr<AActor> FindClosestTarget(const TArray<TObjectPtr<AActor>>& InTargetActorArray, EMyTargetingSwitchDirection InDirection = EMyTargetingSwitchDirection::None) const;
	void OrientCamera() const;
	void OrientMovement() const;
	void FaceLockOnActor() const;
	void LockOnTarget();
	void StopLockOn();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRadius = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FaceLockOnRotationSpeed = 20.0f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UCameraComponent> OwnerCameraComponent;

	UPROPERTY(Transient)
	TObjectPtr<AActor> LockTargetActor;

	bool bIsLockOn = false;
};
