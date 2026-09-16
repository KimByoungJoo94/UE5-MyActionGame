#include "Animation/MyAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMyAnimInstance::UMyAnimInstance()
{

}

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		CharacterMovementComponent = Character->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character && CharacterMovementComponent)
	{
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();

		bShouldMove = GroundSpeed > 3.0f && CharacterMovementComponent->GetCurrentAcceleration().Equals(FVector::ZeroVector) == false;
		bIsFalling = CharacterMovementComponent->IsFalling();
	}
}
