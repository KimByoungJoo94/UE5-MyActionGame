#include "Animation/MyAnimNotify_EquipWeapon.h"
#include "Common/MyGameplayTags.h"
#include "Components/MyCombatComponent.h"
#include "Equipments/MyWeapon.h"


void UMyAnimNotify_EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UMyCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UMyCombatComponent>())
		{
			if (AMyWeapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				bool bCombatEnabld = CombatComponent->IsCombatEnabld();
				FName WeaponSocketName;

				if (MontageActionTag == MyGameplayTags::Character_Action_Equip)
				{
					bCombatEnabld = true;
					WeaponSocketName = MainWeapon->GetEquipSocketName();
				}
				else if (MontageActionTag == MyGameplayTags::Character_Action_Unequip)
				{
					bCombatEnabld = false;
					WeaponSocketName = MainWeapon->GetUnequipSocketName();
				}
				
				CombatComponent->SetCombatEnabld(bCombatEnabld);
				MainWeapon->AttachToOwner(WeaponSocketName);
			}
		}
	}
}
