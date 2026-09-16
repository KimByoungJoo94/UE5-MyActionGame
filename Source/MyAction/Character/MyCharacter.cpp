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

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::OnMoveActionTriggered);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::OnLookActionTriggered);
		
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &AMyCharacter::OnSprintRollingActionTriggered);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &AMyCharacter::OnSprintRollingActionCompleted);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &AMyCharacter::OnSprintRollingActionCanceled);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyCharacter::OnInteractActionStarted);
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

void AMyCharacter::StartSprint()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
		if (AttributeComponent)
		{
			AttributeComponent->ToggleRegenerateStamina(false);
			AttributeComponent->DecreaseStamina(0.1f);
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
