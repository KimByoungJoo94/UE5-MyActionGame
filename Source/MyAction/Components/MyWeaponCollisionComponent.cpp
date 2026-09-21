#include "Components/MyWeaponCollisionComponent.h"


UMyWeaponCollisionComponent::UMyWeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void UMyWeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UMyWeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActorSet.Empty();
	bIsCollisionEnabled = true;
}

void UMyWeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void UMyWeaponCollisionComponent::SetWeaponMesh(TObjectPtr<UPrimitiveComponent> InWeaponMeshComponent)
{
	if (IsValid(InWeaponMeshComponent))
	{
		WeaponMeshComponent = InWeaponMeshComponent;
	}
}

void UMyWeaponCollisionComponent::AddIgnoreActor(TObjectPtr<AActor> InActor)
{	
	if (IsValid(InActor))
	{
		IgonoreActorArray.Add(InActor);
	}
}

void UMyWeaponCollisionComponent::RemoveIgnoreActor(TObjectPtr<AActor> InActor)
{
	IgonoreActorArray.Remove(InActor);
}

bool UMyWeaponCollisionComponent::CanHitActor(TObjectPtr<AActor> InActor)
{
	return AlreadyHitActorSet.Contains(InActor) == false;
}

void UMyWeaponCollisionComponent::CollisionTrace()
{
	if (IsValid(WeaponMeshComponent) == false)
	{
		return;
	}

	TArray<FHitResult> HitResultArray;
	const FVector& StartVector = WeaponMeshComponent->GetSocketLocation(TraceStartSocketName);
	const FVector& EndVector = WeaponMeshComponent->GetSocketLocation(TraceEndSocketName);

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetOwner(), StartVector, EndVector, TraceRadius, TraceObjectTypeArray, false, IgonoreActorArray, DrawDebugType, HitResultArray, true);
	if (bHit)
	{
		for (const FHitResult& HitResult : HitResultArray)
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				if (CanHitActor(HitActor))
				{
					AlreadyHitActorSet.Add(HitActor);

					if (OnHitActor.IsBound())
					{
						OnHitActor.Broadcast(HitResult);
					}
				}
			}
		}
	}
}



