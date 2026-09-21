#include "Animation/MyAnimNotifyState_WeaponCollision.h"
#include "Components/MyCombatComponent.h"
#include "Components/MyWeaponCollisionComponent.h"
#include "Equipments/MyWeapon.h"


void UMyAnimNotifyState_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		if (AActor* OwnerActor = MeshComp->GetOwner())
		{
			if (UMyCombatComponent* OwnerCombatComponent = OwnerActor->GetComponentByClass<UMyCombatComponent>())
			{
				if (AMyWeapon* OwnerWeapon = OwnerCombatComponent->GetMainWeapon())
				{
					if (UMyWeaponCollisionComponent* OwnerWeaponCollisionComponent = OwnerWeapon->GetWeaponCollisionComponent())
					{
						OwnerWeaponCollisionComponent->TurnOnCollision();
					}
				}
			}
		}
	}
}

void UMyAnimNotifyState_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (AActor* OwnerActor = MeshComp->GetOwner())
		{
			if (UMyCombatComponent* OwnerCombatComponent = OwnerActor->GetComponentByClass<UMyCombatComponent>())
			{
				if (AMyWeapon* OwnerWeapon = OwnerCombatComponent->GetMainWeapon())
				{
					if (UMyWeaponCollisionComponent* OwnerWeaponCollisionComponent = OwnerWeapon->GetWeaponCollisionComponent())
					{
						OwnerWeaponCollisionComponent->TurnOffCollision();
					}
				}
			}
		}
	}
}
