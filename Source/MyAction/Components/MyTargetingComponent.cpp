#include "Components/MyTargetingComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/MyTargetingInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Common/MyDefines.h"

UMyTargetingComponent::UMyTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCameraComponent = OwnerCharacter->GetComponentByClass<UCameraComponent>();
	}
}

void UMyTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLockOn == false ||
		IsValid(OwnerCharacter) == false || 
		IsValid(LockTargetActor) == false)
	{
		return;
	}

	const float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), LockTargetActor->GetActorLocation());

	if (IMyTargetingInterface* TargetingInterface = Cast<IMyTargetingInterface>(LockTargetActor))
	{		
		if (TargetingInterface->CanTargeting() == false || Distance > TargetingRadius)
		{
			StopLockOn();
		}
		else
		{
			FaceLockOnActor();
		}
	}
}

void UMyTargetingComponent::ToggleLockOnff()
{
	if (bIsLockOn)
	{
		StopLockOn();
	}
	else
	{
		LockOnTarget();
	}
}

void UMyTargetingComponent::SwitchLockTargetActor(EMyTargetingSwitchDirection InDirection)
{
	if (IsValid(LockTargetActor))
	{
		if (IMyTargetingInterface* OldTargetingInterface = Cast<IMyTargetingInterface>(LockTargetActor))
		{			
			OldTargetingInterface->OnTargeting(false);

			TArray<TObjectPtr<AActor>> TargetActorArray;
			FindTargetArray(TargetActorArray);

			AActor* NewTargetActor = FindClosestTarget(TargetActorArray, InDirection);

			if (IsValid(NewTargetActor))
			{
				if (IMyTargetingInterface* NewTargetingInterface = Cast<IMyTargetingInterface>(NewTargetActor))
				{
					LockTargetActor = NewTargetActor;
					NewTargetingInterface->OnTargeting(true);
				}
			}
			else
			{
				StopLockOn();
			}
		}
	}
}

void UMyTargetingComponent::FindTargetArray(OUT TArray<TObjectPtr<AActor>>& OutTargetActorArray) const
{
	TArray<FHitResult> HitResultArray;
	const FVector& ActorLocation = OwnerCharacter->GetActorLocation();

	TArray<AActor*> IgnoreActorArray;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(MY_COLLISION_OBJECT_TARGETING));

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetOwner(), ActorLocation, ActorLocation, TargetingRadius, ObjectTypeArray, false, IgnoreActorArray, DrawDebugType, HitResultArray, true);
	if (bHit)
	{
		for (const FHitResult& HitResult : HitResultArray)
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				if (IMyTargetingInterface* TargetingInterface = Cast<IMyTargetingInterface>(HitResult.GetActor()))
				{
					if (TargetingInterface->CanTargeting())
					{
						OutTargetActorArray.Add(HitActor);
					}
				}
			}
		}
	}
}

TObjectPtr<AActor> UMyTargetingComponent::FindClosestTarget(const TArray<TObjectPtr<AActor>>& InTargetActorArray, EMyTargetingSwitchDirection InDirection /*= EMyTargetingSwitchDirection::None*/) const
{
	float TargetCompareValue = 0.f;
	AActor* ClosestTarget = nullptr;

	for (const AActor* TargetActor : InTargetActorArray)
	{
		if (InDirection != EMyTargetingSwitchDirection::None && LockTargetActor == TargetActor)
		{
			continue;
		}

		FHitResult HitResult;
		const FVector& StartLocation = OwnerCameraComponent->GetComponentLocation();
		const FVector& EndLocation = TargetActor->GetActorLocation();
		TArray<AActor*> IgnoreActorArray;

		const bool bHit = UKismetSystemLibrary::LineTraceSingle(GetOwner(), StartLocation, EndLocation, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoreActorArray, DrawDebugType, HitResult, true);
		if (bHit)
		{			
			if (InDirection == EMyTargetingSwitchDirection::Left)
			{				
				if (FVector::DotProduct(OwnerCameraComponent->GetRightVector(), HitResult.Normal) > 0.f == false)
				{
					continue;
				}
			}

			if (InDirection == EMyTargetingSwitchDirection::Right)
			{
				if (FVector::DotProduct(OwnerCameraComponent->GetRightVector(), HitResult.Normal) < 0.f == false)
				{
					continue;
				}
			}

			AActor* HitActor = HitResult.GetActor();
			if (IsValid(HitActor) == false)
			{
				continue;
			}

			const FRotator& LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), HitActor->GetActorLocation());
			float CheckValue = FVector::DotProduct(OwnerCameraComponent->GetForwardVector(), LookAtRotation.Vector());

			if (CheckValue > TargetCompareValue)
			{
				TargetCompareValue = CheckValue;
				ClosestTarget = HitActor;
			}
		}
	}

	return ClosestTarget;
}

void UMyTargetingComponent::OrientCamera() const
{
	if (IsValid(OwnerCharacter))
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovementComponent->bOrientRotationToMovement = false;
			CharacterMovementComponent->bUseControllerDesiredRotation = true;
		}
	}
}

void UMyTargetingComponent::OrientMovement() const
{
	if (IsValid(OwnerCharacter))
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovementComponent->bOrientRotationToMovement = true;
			CharacterMovementComponent->bUseControllerDesiredRotation = false;
		}
	}
}

void UMyTargetingComponent::FaceLockOnActor() const
{
	if (OwnerCharacter && LockTargetActor)
	{
		if (AController* CharacterController = OwnerCharacter->GetController())
		{
			const FRotator& CurrentControlRotation = OwnerCharacter->GetControlRotation();
			const FVector& TargetLocation = LockTargetActor->GetActorLocation() - FVector(0.f, 0.f, 150.f);
			const FRotator& TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), TargetLocation);
			const FRotator& InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), FaceLockOnRotationSpeed);

			CharacterController->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, CurrentControlRotation.Roll));
		}
	}
}

void UMyTargetingComponent::LockOnTarget()
{
	TArray<TObjectPtr<AActor>> OutTargetArray;
	FindTargetArray(OutTargetArray);

	AActor* TargetActor = FindClosestTarget(OutTargetArray);

	if (IsValid(TargetActor))
	{
		LockTargetActor = TargetActor;
		bIsLockOn = true;
		if (IMyTargetingInterface* TargetingInterface = Cast<IMyTargetingInterface>(LockTargetActor))
		{
			TargetingInterface->OnTargeting(bIsLockOn);
		}

		OrientCamera();
	}
}

void UMyTargetingComponent::StopLockOn()
{
	bIsLockOn = false;
	if (IMyTargetingInterface* TargetingInterface = Cast<IMyTargetingInterface>(LockTargetActor))
	{
		TargetingInterface->OnTargeting(bIsLockOn);
	}

	LockTargetActor = nullptr;
	OrientMovement();
}

