#include "Character/MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

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
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->SetRelativeRotation(FRotator( -30.0f, 0.0f, 0.0f));
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();	
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

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::OnMoveAction);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::OnLookAction);
	}
}

void AMyCharacter::OnMoveAction(const FInputActionValue& InValue)
{
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

void AMyCharacter::OnLookAction(const FInputActionValue& InValue)
{	
	if (GetController())
	{	
		const FVector2D& MovementVector = InValue.Get<FVector2D>();
		AddControllerYawInput(MovementVector.X);
		AddControllerPitchInput(MovementVector.Y);
	}
}