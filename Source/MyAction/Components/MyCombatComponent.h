#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/MyGameplayTags.h"
#include "MyCombatComponent.generated.h"

class AMyWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTION_API UMyCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMyCombatComponent();

protected:
	virtual void BeginPlay() override;

public:		
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMainWeapon(AMyWeapon* InWeapon);

	FORCEINLINE bool IsCombatEnabld() const { return bCombatEnabld; }
	FORCEINLINE void SetCombatEnabld(bool bInEnabled) { bCombatEnabld = bInEnabled; }
	FORCEINLINE TObjectPtr<AMyWeapon> GetMainWeapon() { return MainWeapon; }
	FORCEINLINE void SetLastAttackGameplayTag(const FGameplayTag InAttackGameplayTag) { LastAttackGameplayTag = InAttackGameplayTag; }
	FORCEINLINE const FGameplayTag& GetLastAttackGameplayTag() const { return LastAttackGameplayTag; }

protected:
	UPROPERTY()
	TObjectPtr<AMyWeapon> MainWeapon;

	UPROPERTY(EditAnywhere)
	bool bCombatEnabld;

	UPROPERTY(Transient)
	FGameplayTag LastAttackGameplayTag;
};
