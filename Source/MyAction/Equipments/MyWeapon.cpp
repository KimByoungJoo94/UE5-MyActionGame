#include "Equipments/MyWeapon.h"
#include "Components/MyCombatComponent.h"
#include "Components/MyWeaponCollisionComponent.h"
#include "Assets/MyMontageActionDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


AMyWeapon::AMyWeapon()
{
	WeaponCollisionComponent = CreateDefaultSubobject<UMyWeaponCollisionComponent>(TEXT("WeaponCollisionComponent"));
	WeaponCollisionComponent->GetOnHitActor().AddUObject(this, &ThisClass::OnHitActor);

	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Light, 7.0f);
	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Running, 12.0f);
	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Special, 15.0f);
	StaminaCostMap.Add(MyGameplayTags::Character_Attack_Heavy, 20.0f);
		
	DamageMultiplierMap.Add(MyGameplayTags::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Add(MyGameplayTags::Character_Attack_Special, 1.8f);
	DamageMultiplierMap.Add(MyGameplayTags::Character_Attack_Heavy, 2.1f);
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

			if (WeaponCollisionComponent)
			{
				WeaponCollisionComponent->SetWeaponMesh(MeshComponent);
				WeaponCollisionComponent->AddIgnoreActor(OwnerActor);
			}
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

float AMyWeapon::GetAttackDamage() const
{
	if (IsValid(OwnerCombatComponent))
	{	
		const FGameplayTag& LastAttackGameplayTag = OwnerCombatComponent->GetLastAttackGameplayTag();
		if (const float* FindDamage = DamageMultiplierMap.Find(LastAttackGameplayTag))
		{
			return BaseDamage * (*FindDamage);
		}
	}

	return BaseDamage;
}

void AMyWeapon::OnHitActor(const FHitResult& InHitResult)
{
	if (AActor* TargetActor = InHitResult.GetActor())
	{
		if (AActor* OwnerActor = GetOwner())
		{
			const FVector& DamagerDirection = OwnerActor->GetActorForwardVector();
			const float AttackDamage = GetAttackDamage();

			UGameplayStatics::ApplyPointDamage(TargetActor, AttackDamage, DamagerDirection, InHitResult, OwnerActor->GetInstigatorController(), this, nullptr);
		}
	}
}
