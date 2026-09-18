#include "Character/MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/MyAttributeComponent.h"
#include "Components/MyStateComponent.h"
#include "Components/MyCombatComponent.h"
#include "UI/MyPlayHUDWidget.h"
#include "Common/MyGameplayTags.h"
#include "Interfaces/MyInteractionInterface.h"
#include "Equipments/MyWeapon.h"


AMyCharacter::AMyCharacter()
{	
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{	
		CharacterMovementComponent->bOrientRotationToMovement = true;
		CharacterMovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
		CharacterMovementComponent->MaxWalkSpeed = 500.0f;
		CharacterMovementComponent->BrakingDecelerationWalking = 2000.0f;
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->SetRelativeRotation(FRotator( -30.0f, 0.0f, 0.0f));
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<UMyAttributeComponent>(TEXT("AttributeComponent"));
	StateComponent = CreateDefaultSubobject<UMyStateComponent>(TEXT("StateComponent"));
	CombatComponent = CreateDefaultSubobject<UMyCombatComponent>(TEXT("CombatComponent"));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();	

	if (PlayHUDWidgetClass)
	{
		PlayHUDWidget = CreateWidget<UMyPlayHUDWidget>(GetWorld(), PlayHUDWidgetClass);
		if (PlayHUDWidget)
		{
			PlayHUDWidget->AddToViewport();
		}
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem >(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}
}

bool AMyCharacter::IsMoving() const
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{		
		return CharacterMovementComponent->Velocity.Size2D() > 3.0f && 
			CharacterMovementComponent->GetCurrentAcceleration().Equals(FVector::ZeroVector) == false;
	}

	return false;
}

bool AMyCharacter::CanToggleCombat() const
{	
	if (StateComponent)
	{
		FGameplayTagContainer GameplayTagContainer;
		GameplayTagContainer.AddTag(MyGameplayTags::Character_State_Attacking);
		GameplayTagContainer.AddTag(MyGameplayTags::Character_State_Rolling);
		GameplayTagContainer.AddTag(MyGameplayTags::Character_State_GeneralAction);

		return StateComponent->IsCurrentStateEqualToAny(GameplayTagContainer) == false;
	}

	return false;
}

const FGameplayTag AMyCharacter::GetAttackPerform() const
{
	if (IsSprinting())
	{
		return MyGameplayTags::Character_Attack_Running;
	}

	return MyGameplayTags::Character_Attack_Light;
}

bool AMyCharacter::CanPerformAttack(const FGameplayTag& InAttackGameplayTag) const
{
	if (IsValid(StateComponent) == false || 
		IsValid(AttributeComponent) == false ||
		IsValid(CombatComponent) == false || 
		IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	FGameplayTagContainer GameplayTagContainer;
	GameplayTagContainer.AddTag(MyGameplayTags::Character_State_Rolling);
	GameplayTagContainer.AddTag(MyGameplayTags::Character_State_GeneralAction);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(InAttackGameplayTag);

	return StateComponent->IsCurrentStateEqualToAny(GameplayTagContainer) == false && 
		CombatComponent->IsCombatEnabld() &&
		AttributeComponent->HasEnounghStamina(StaminaCost);
}

void AMyCharacter::ResetComboAttack()
{
	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

void AMyCharacter::EnableComboAttack()
{
	bCanComboInput = true;
}

void AMyCharacter::DisableComboAttack()
{
	if (CombatComponent)
	{
		bCanComboInput = false;

		if (bSavedComboInput)
		{
			bSavedComboInput = false;
			++ComboCounter;
			AttackByGameplayTag(CombatComponent->GetLastAttackGameplayTag());
		}
	}
}

void AMyCharacter::FinishComboAttack(const float InDelay)
{
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetComboAttack, InDelay, false);
	}
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveActionTriggered);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::OnLookActionTriggered);
		
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ThisClass::OnSprintRollingActionTriggered);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ThisClass::OnSprintRollingActionCompleted);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ThisClass::OnSprintRollingActionCanceled);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::OnInteractActionStarted);
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ThisClass::OnToggleCombatActionStarted);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::OnAttackActionStarted);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ThisClass::OnAttackActionCanceled);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::OnAttackActionTriggered);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::OnHeavyAttackActionStarted);
	}
}

void AMyCharacter::OnMoveActionTriggered(const FInputActionValue& InValue)
{
	if (StateComponent && StateComponent->IsMovementInputEnabled() == false)
	{
		return;
	}

	if (GetController())
	{
		const FVector2D& MovementVector = InValue.Get<FVector2D>();
		
		const FRotator& ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
		const FVector& FowardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector& RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AMyCharacter::OnLookActionTriggered(const FInputActionValue& InValue)
{	
	if (GetController())
	{	
		const FVector2D& MovementVector = InValue.Get<FVector2D>();
		AddControllerYawInput(MovementVector.X);
		AddControllerPitchInput(MovementVector.Y);
	}
}

void AMyCharacter::OnSprintRollingActionTriggered()
{
	if (AttributeComponent && AttributeComponent->HasEnounghStamina(5.0f) && IsMoving())
	{
		StartSprint();
	}
	else
	{
		StopSprint();
	}
}

void AMyCharacter::OnSprintRollingActionCompleted()
{
	StopSprint();
}

void AMyCharacter::OnSprintRollingActionCanceled()
{
	DoRolling();
}

void AMyCharacter::OnInteractActionStarted()
{
	DoInteraction();
}

void AMyCharacter::OnToggleCombatActionStarted()
{
	if (CanToggleCombat())
	{
		DoToggleCombat();
	}
}

void AMyCharacter::OnAttackActionStarted()
{
	AutoToggleCombat();
}

void AMyCharacter::OnAttackActionCanceled()
{
	DoAttack();
}

void AMyCharacter::OnAttackActionTriggered()
{
	DoSpecialAttack();
}

void AMyCharacter::OnHeavyAttackActionStarted()
{
	DoHeavyAttack();
}

void AMyCharacter::StartSprint()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
		if (AttributeComponent)
		{
			AttributeComponent->ToggleRegenerateStamina(false);
			AttributeComponent->DecreaseStamina(0.1f);
			bSprinting = true;
		}
	}
}

void AMyCharacter::StopSprint()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = NormalSpeed;
		if (AttributeComponent)
		{
			AttributeComponent->ToggleRegenerateStamina(true);
			bSprinting = false;
		}
	}
}

void AMyCharacter::DoRolling()
{
	if (StateComponent && 
		AttributeComponent && 
		AttributeComponent->HasEnounghStamina(15.0f))
	{
		AttributeComponent->ToggleRegenerateStamina(false);
		StateComponent->ToggleMovementInput(false, 0.f);
		AttributeComponent->DecreaseStamina(15.0f);

		PlayAnimMontage(RollingAnimMontage);

		StateComponent->SetState(MyGameplayTags::Character_State_Rolling);
		AttributeComponent->ToggleRegenerateStamina(true, 1.5f);
	}
}

void AMyCharacter::DoInteraction()
{
	FHitResult HitResult;
	const FVector StartVector = GetActorLocation();
	const FVector EndVector = StartVector;
	const float Radius = 100.0f;

	TArray<AActor*> IgnoreActorArray;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(this, StartVector, EndVector, Radius, ObjectTypeArray, false, IgnoreActorArray, EDrawDebugTrace::ForDuration, HitResult, true);
	if (bHit)
	{
		if (IMyInteractionInterface* HitActorInterface = Cast<IMyInteractionInterface>(HitResult.GetActor()))
		{
			HitActorInterface->Interact(this);
		}
	}
}

void AMyCharacter::DoToggleCombat()
{
	if (CombatComponent && StateComponent)
	{
		if (const AMyWeapon* MainWeapon = CombatComponent->GetMainWeapon())
		{			
			StateComponent->SetState(MyGameplayTags::Character_State_GeneralAction);

			if (CombatComponent->IsCombatEnabld())
			{
				PlayAnimMontage(MainWeapon->GetMontage(MyGameplayTags::Character_Action_Unequip, 0));
			}
			else
			{
				PlayAnimMontage(MainWeapon->GetMontage(MyGameplayTags::Character_Action_Equip, 0));
			}
		}
	}
}

void AMyCharacter::AutoToggleCombat()
{
	if (CombatComponent && CombatComponent->IsCombatEnabld() == false)
	{
		DoToggleCombat();
	}
}

void AMyCharacter::DoAttack()
{
	const FGameplayTag& AttackGameplayTag = GetAttackPerform();
	if (CanPerformAttack(AttackGameplayTag))
	{
		DoComboAttack(AttackGameplayTag);
	}
}

void AMyCharacter::DoSpecialAttack()
{	
	if (CanPerformAttack(MyGameplayTags::Character_Attack_Special))
	{
		DoComboAttack(MyGameplayTags::Character_Attack_Special);
	}
}

void AMyCharacter::DoHeavyAttack()
{
	AutoToggleCombat();

	if (CanPerformAttack(MyGameplayTags::Character_Attack_Heavy))
	{
		DoComboAttack(MyGameplayTags::Character_Attack_Heavy);
	}
}

void AMyCharacter::DoComboAttack(const FGameplayTag& InAttackGameplayTag)
{
	if (IsValid(StateComponent))
	{
		if (StateComponent->IsCurrentState(InAttackGameplayTag) == false)
		{
			if (bComboSequenceRunning && bCanComboInput)
			{
				++ComboCounter;
			}
			else
			{
				ResetComboAttack();
				bComboSequenceRunning = true;
			}

			AttackByGameplayTag(InAttackGameplayTag);
			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().ClearTimer(ComboResetTimerHandle);
			}
		}
		else if (bCanComboInput)
		{
			bSavedComboInput = true;
		}
	}
}

void AMyCharacter::AttackByGameplayTag(const FGameplayTag& InAttackGameplayTag)
{
	if (IsValid(StateComponent) == false ||
		IsValid(AttributeComponent) == false ||
		IsValid(CombatComponent) == false)
	{
		return;
	}

	if (const AMyWeapon* MainWeapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(MyGameplayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);
		CombatComponent->SetLastAttackGameplayTag(InAttackGameplayTag);
		AttributeComponent->ToggleRegenerateStamina(false);
		
		UAnimMontage* AttackAnimMontage = MainWeapon->GetMontage(InAttackGameplayTag, ComboCounter);
		if (AttackAnimMontage == nullptr)
		{
			ComboCounter = 0;
			AttackAnimMontage = MainWeapon->GetMontage(InAttackGameplayTag, ComboCounter);
		}

		PlayAnimMontage(AttackAnimMontage);

		const float StaminaCost = MainWeapon->GetStaminaCost(InAttackGameplayTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleRegenerateStamina(true, 1.5f);
	}
}



