#pragma once

#include "CoreMinimal.h"
#include "Equipments/MyEquipment.h"
#include "Common/MyGameplayTags.h"
#include "MyWeapon.generated.h"

class UMyCombatComponent;
class UMyWeaponCollisionComponent;
class UMyMontageActionDataAsset;
class UAnimMontage;
struct FGameplayTag;
struct FHitResult;

UCLASS()
class MYACTION_API AMyWeapon : public AMyEquipment
{
	GENERATED_BODY()

public:
	AMyWeapon();

	virtual void Equip() override;
	virtual void Unequip() override;

	const TObjectPtr<UAnimMontage> GetMontage(const FGameplayTag& InGameplayTag, const int32 InIndex) const;
	const FName GetEquipSocketName() const { return EquipSocketName; }
	const FName GetUnequipSocketName() const { return UnequipSocketName; }
	const float GetStaminaCost(const FGameplayTag& InGameplayTag) const { return StaminaCostMap.FindRef(InGameplayTag); }
	float GetAttackDamage() const;
	TObjectPtr<UMyWeaponCollisionComponent> GetWeaponCollisionComponent() const { return WeaponCollisionComponent; }

	void OnHitActor(const FHitResult& InHitResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAction|Socket")
	FName EquipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAction|Socket")
	FName UnequipSocketName;

	UPROPERTY(Transient)
	TObjectPtr<UMyCombatComponent> OwnerCombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAction|Animation")
	TObjectPtr<UMyMontageActionDataAsset> MontageActionDataAsset;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMyWeaponCollisionComponent> WeaponCollisionComponent;

	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.0f;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;
};
