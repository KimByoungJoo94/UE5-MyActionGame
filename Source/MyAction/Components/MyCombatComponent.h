#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	void SetWeapon(AMyWeapon* InWeapon);

protected:
	UPROPERTY()
	TObjectPtr<AMyWeapon> MainWeapon;
};
