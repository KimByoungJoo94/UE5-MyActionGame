#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEquipment.generated.h"

class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class MYACTION_API AMyEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyEquipment();

protected:	
	virtual void BeginPlay() override;

public:		
	virtual void Tick(float DeltaTime) override;

	virtual void Equip() {}
	virtual void Unequip() {}
	virtual void AttachToOwner(const FName InSocketName);

	TObjectPtr<UStaticMesh> GetMeshAsset();

protected:
	UPROPERTY(EditAnywhere, Category = "MyAction|Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
};
