#include "Equipments/MyWeapon.h"
#include "Components/MyCombatComponent.h"
#include "Assets/MyMontageActionDataAsset.h"

AMyWeapon::AMyWeapon()
{
	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Light, 7.0f);
	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Running, 12.0f);
	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Special, 15.0f);
	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Heavy, 20.0f);
}

void AMyWeapon::Equip()
{
	Super::Equip();

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerCombatComponent = OwnerActor->GetComponentByClass<UMyCombatComponent>();
		if (OwnerCombatComponent)
		{
			OwnerCombatComponent->SetMainWeapon(this);

			AttachToOwner(OwnerCombatComponent->IsCombatEnabld() ? EquipSocketName : UnequipSocketName);
		}
	}
}

void AMyWeapon::Unequip()
{
	

}

const TObjectPtr<UAnimMontage> AMyWeapon::GetMontage(const FGameplayTag& InGameplayTag, const int32 InIndex) const
{
	if (MontageActionDataAsset)
	{
		return MontageActionDataAsset->GetMontage(InGameplayTag, InIndex);
	}

	return nullptr;
}
