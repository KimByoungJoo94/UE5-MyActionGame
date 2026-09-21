#include "MyAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MyCharacter.h"
#include "Components/MyStateComponent.h"


UMyAnimInstance::UMyAnimInstance()
{

}

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwningCharacter)
	{
		OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwningCharacter && OwningCharacterMovementComponent)
	{	
		Velocity = OwningCharacterMovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();

		bShouldMove = GroundSpeed > 3.0f && OwningCharacterMovementComponent->GetCurrentAcceleration().Equals(FVector::ZeroVector) == false;
		bIsFalling = OwningCharacterMovementComponent->IsFalling();

		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation());
	}
}

void UMyAnimInstance::AnimNotify_ResetMovementInput()
{
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OwningCharacter))
	{
		if (UMyStateComponent* StateComponent = MyCharacter->GetStateComponent())
		{
			StateComponent->ToggleMovementInput(true, 0.0f);
		}
	}
}

void UMyAnimInstance::AnimNotify_ResetState()
{
	if (AMyCharacter* MyCharacter = Cast<AMyCharacter>(OwningCharacter))
	{
		if (UMyStateComponent* StateComponent = MyCharacter->GetStateComponent())
		{
			StateComponent->ClearState();
		}
	}
}