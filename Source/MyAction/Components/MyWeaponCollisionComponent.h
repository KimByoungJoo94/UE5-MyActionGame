#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyWeaponCollisionComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult& InHitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYACTION_API UMyWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	UMyWeaponCollisionComponent();

protected:
	virtual void BeginPlay() override;

public:		
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	FOnHitActor& GetOnHitActor() { return OnHitActor; }
	
	void TurnOnCollision();
	void TurnOffCollision();
	void SetWeaponMesh(TObjectPtr<UPrimitiveComponent> InWeaponMeshComponent);
	void AddIgnoreActor(TObjectPtr<AActor> InActor);
	void RemoveIgnoreActor(TObjectPtr<AActor> InActor);
	bool CanHitActor(TObjectPtr<AActor> InActor);
	void CollisionTrace();

protected:
	UPROPERTY(EditAnywhere, Category = "MyAction|Collision")
	FName TraceStartSocketName;

	UPROPERTY(EditAnywhere, Category = "MyAction|Collision")
	FName TraceEndSocketName;

	UPROPERTY(EditAnywhere, Category = "MyAction|Collision")
	float TraceRadius = 20.f;

	UPROPERTY(EditAnywhere, Category = "MyAction|Collision")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypeArray;

	UPROPERTY(EditAnywhere, Category = "MyAction|Collision")
	TArray<TObjectPtr<AActor>> IgonoreActorArray;

	UPROPERTY(EditAnywhere, Category = "MyAction|Collision")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	UPROPERTY(Transient)
	TObjectPtr<UPrimitiveComponent> WeaponMeshComponent;

	UPROPERTY(Transient)
	TSet<TObjectPtr<AActor>> AlreadyHitActorSet;

	bool bIsCollisionEnabled = false;

protected:
	FOnHitActor OnHitActor;
};
