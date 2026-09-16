#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/MyInteractionInterface.h"
#include "MyPickupItem.generated.h"

class UStaticMeshComponent;

UCLASS()
class MYACTION_API AMyPickupItem : public AActor, public IMyInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AMyPickupItem();

protected:	
	virtual void BeginPlay() override;

public:		
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* InTargetActor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyAction|Item")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

};
