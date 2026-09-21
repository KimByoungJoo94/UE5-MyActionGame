#include "Character/MyEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/MyAttributeComponent.h"
#include "Components/MyStateComponent.h"
#include "Common/MyGameplayTags.h"
#include "Common/MyDefines.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"


AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	if (UCapsuleComponent* CharacterCapsule = GetCapsuleComponent())
	{
		CharacterCapsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		CharacterMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	AttributeComponent = CreateDefaultSubobject<UMyAttributeComponent>(TEXT("AttributeComponent"));
	if (AttributeComponent)
	{
		AttributeComponent->GetOnDeath().AddUObject(this, &ThisClass::OnDeath);
	}

	StateComponent = CreateDefaultSubobject<UMyStateComponent>(TEXT("StateComponent"));

	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphereComponent"));
	if (TargetingSphereComponent)
	{
		TargetingSphereComponent->SetupAttachment(GetRootComponent());
		TargetingSphereComponent->SetCollisionObjectType(MY_COLLISION_OBJECT_TARGETING);
		TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
	
	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	if (LockOnWidgetComponent)
	{
		LockOnWidgetComponent->SetupAttachment(GetRootComponent());
		LockOnWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
		LockOnWidgetComponent->SetDrawSize(FVector2D(30.0f, 30.0f));
		LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		LockOnWidgetComponent->SetVisibility(false);
	}
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();	
}

void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMyEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent))
		{
			const FVector& ShotDirection = PointDamageEvent->ShotDirection;
			const FVector& ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
			const FVector& ImpactNormal = PointDamageEvent->HitInfo.ImpactNormal;
			const FVector& HitLocation = PointDamageEvent->HitInfo.Location;

			ImpactEffect(ImpactPoint);
			HitReaction(EventInstigator->GetPawn());
		}
	}

	return ActualDamage;
}

void AMyEnemy::OnDeath()
{
	if (UCapsuleComponent* CharacterCapsule = GetCapsuleComponent())
	{
		CharacterCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		CharacterMesh->SetCollisionProfileName(TEXT("Ragdoll"));
		CharacterMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		CharacterMesh->SetSimulatePhysics(true);
	}
}

void AMyEnemy::OnTargeting(bool bInTargeting)
{
	if (LockOnWidgetComponent)
	{
		LockOnWidgetComponent->SetVisibility(bInTargeting);
	}
}

bool AMyEnemy::CanTargeting()
{
	if (StateComponent)
	{
		/*
		FGameplayTagContainer GameplayTagContainer;
		GameplayTagContainer.AddTag(MyGameplayTags::Character_State_Death);
		return StateComponent->IsCurrentStateEqualToAny(GameplayTagContainer); == false
		*/

		return StateComponent->IsCurrentState(MyGameplayTags::Character_State_Death) == false;
	}

	return false;
}

void AMyEnemy::ImpactEffect(const FVector& InLocation)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, InLocation);
	}

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, InLocation);
	}
}

void AMyEnemy::HitReaction(const AActor* InAttackActor)
{
	if (UAnimMontage* HitReactionAnimMantage = GetHitReactionAnimMantage(InAttackActor))
	{
		PlayAnimMontage(HitReactionAnimMantage);
	}
}

TObjectPtr<UAnimMontage> AMyEnemy::GetHitReactionAnimMantage(const AActor* InAttackActor)
{
	if (IsValid(InAttackActor) == false)
	{
		return nullptr;
	}

	const FRotator& LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InAttackActor->GetActorLocation());
	const FRotator& DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	const float DeltaZ = DeltaRotation.Yaw;
	
	UAnimMontage* HitMontage = nullptr;
	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitMontage = HitReactionFrontMontage;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitMontage = HitReactionLeftMontage;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitMontage = HitReactionBackMontage;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitMontage = HitReactionRightMontage;
	}

	return HitMontage;
}

