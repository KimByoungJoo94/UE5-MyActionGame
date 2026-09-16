#pragma once

#include "CoreMinimal.h"
#include "Equipments/MyEquipment.h"
#include "MyWeapon.generated.h"

class UMyCombatComponent;

UCLASS()
class MYACTION_API AMyWeapon : public AMyEquipment
{
	GENERATED_BODY()

public:
	virtual void Equip() override;
	virtual void Unequip() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAction|Socket")
	FName EquipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAction|Socket")
	FName UnequipSocketName;

	UPROPERTY()
	TObjectPtr<UMyCombatComponent> OwnerCombatComponent;
};
