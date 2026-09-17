#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MyInteractionInterface.h"
#include "MyPickupItem.generated.h"

class UStaticMeshComponent;
class AMyEquipment;

UCLASS()
class MYACTION_API AMyPickupItem : public AActor, public IMyInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AMyPickupItem();

protected:	
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:		
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* InTargetActor) override;

	void SetTargetItemClass(const TSubclassOf<AMyEquipment>& InTargetItemClass) { TargetItemClass = InTargetItemClass; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyAction|Item")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyAction|Item")
	TSubclassOf<AMyEquipment> TargetItemClass;
};
