#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/MyTargetingInterface.h"
#include "MyEnemy.generated.h"

class UMyAttributeComponent;
class UMyStateComponent;
class USphereComponent;
class UWidgetComponent;
class USoundCue;
class UParticleSystem;


UCLASS()
class MYACTION_API AMyEnemy : public ACharacter, public IMyTargetingInterface
{
	GENERATED_BODY()

public:
	AMyEnemy();

protected:	
	virtual void BeginPlay() override;

public:		
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void OnDeath();

	virtual void OnTargeting(bool bInTargeting) override;
	virtual bool CanTargeting() override;

protected:
	void ImpactEffect(const FVector& InLocation);
	void HitReaction(const AActor* InAttackActor);
	TObjectPtr<UAnimMontage> GetHitReactionAnimMantage(const AActor* InAttackActor);

protected:
	UPROPERTY(VisibleAnywhere, Category = "MyAction|Attribute")
	TObjectPtr<UMyAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Attribute")
	TObjectPtr<UMyStateComponent> StateComponent;

	UPROPERTY(EditAnywhere, Category = "MyAction|Effect")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "MyAction|Effect")
	TObjectPtr<UParticleSystem> ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "MyAction|Montage|HitReaction")
	TObjectPtr<UAnimMontage> HitReactionFrontMontage;

	UPROPERTY(EditAnywhere, Category = "MyAction|Montage|HitReaction")
	TObjectPtr<UAnimMontage> HitReactionBackMontage;

	UPROPERTY(EditAnywhere, Category = "MyAction|Montage|HitReaction")
	TObjectPtr<UAnimMontage> HitReactionLeftMontage;

	UPROPERTY(EditAnywhere, Category = "MyAction|Montage|HitReaction")
	TObjectPtr<UAnimMontage> HitReactionRightMontage;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Targeting")
	TObjectPtr<USphereComponent> TargetingSphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "MyAction|Targeting")
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;
};
