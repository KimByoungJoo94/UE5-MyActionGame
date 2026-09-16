#include "Equipments/MyWeapon.h"
#include "Components/MyCombatComponent.h"

void AMyWeapon::Equip()
{
	Super::Equip();

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerCombatComponent = OwnerActor->GetComponentByClass<UMyCombatComponent>();
		if (OwnerCombatComponent)
		{
			OwnerCombatComponent->SetWeapon(this);
			AttachToOwner(EquipSocketName);
		}
	}
}

void AMyWeapon::Unequip()
{
}
