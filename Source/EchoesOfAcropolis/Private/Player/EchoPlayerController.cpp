// Copyright Echo of Acropolis. All Rights Reserved.


#include "Player/EchoPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Input/EchoInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

void AEchoPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetEchoAbilitySystemComponent() == nullptr)
	{
		return;
	}

	GetEchoAbilitySystemComponent()->AbilityTagPressed(InputTag);
	
}

void AEchoPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetEchoAbilitySystemComponent() == nullptr)
	{
		return;
	}

	GetEchoAbilitySystemComponent()->AbilityTagReleased(InputTag);
}

void AEchoPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetEchoAbilitySystemComponent() == nullptr)
	{
		return;
	}

	GetEchoAbilitySystemComponent()->AbilityTagHeld(InputTag);
}

UEchoAbilitySystemComponent* AEchoPlayerController::GetEchoAbilitySystemComponent()
{
	if (!EchoAbilitySystemComponent)
	{
		EchoAbilitySystemComponent = CastChecked<UEchoAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return EchoAbilitySystemComponent;
}

void AEchoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultMappingContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AEchoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEchoInputComponent* EchoInputComponent = Cast<UEchoInputComponent>(InputComponent);
	check(EchoInputComponent);

	// Bind the jump action
	EchoInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEchoPlayerController::Jump);
	EchoInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEchoPlayerController::StopJumping);

	// Bind the move action
	EchoInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEchoPlayerController::Move);

	// Bind the look action
	EchoInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEchoPlayerController::Look);

	// Bind the ability actions
	EchoInputComponent->BindAbilityActions(EchoInputConfig, this, &AEchoPlayerController::AbilityInputTagPressed, &AEchoPlayerController::AbilityInputTagReleased, &AEchoPlayerController::AbilityInputTagHeld);
}

void AEchoPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	
	// Add forward movement
	const FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());
	const FVector UpVector = UKismetMathLibrary::GetUpVector(GetCharacter()->GetCapsuleComponent()->GetComponentRotation());

	const FVector Direction = FVector::CrossProduct(RightVector, UpVector);
	const FVector NormalizedDirection = UKismetMathLibrary::Normal(Direction);
		
	GetCharacter()->AddMovementInput(NormalizedDirection, MovementVector.Y);

	// Add right movement
	const FRotator Rotation = GetControlRotation();
	const FVector RightDirection = UKismetMathLibrary::GetRightVector(Rotation);
		
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void AEchoPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void AEchoPlayerController::Jump(const FInputActionValue& Value)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->Jump();
	}
}

void AEchoPlayerController::StopJumping(const FInputActionValue& Value)
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}

// void AEchoPlayerController::MoveForward(float Value)
// {
// 	if (Value == 0.0f)
// 	{
// 		return;
// 	}
// 	
// 	const FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());
// 	const FVector UpVector = UKismetMathLibrary::GetUpVector(GetCharacter()->GetCapsuleComponent()->GetComponentRotation());
//
// 	const FVector Direction = FVector::CrossProduct(RightVector, UpVector);
// 	const FVector NormalizedDirection = UKismetMathLibrary::Normal(Direction);
// 		
// 	GetCharacter()->AddMovementInput(NormalizedDirection, Value);
// }
//
// void AEchoPlayerController::MoveRight(float Value)
// {
// 	if (Value == 0.0f)
// 	{
// 		return;
// 	}
// 	
// 	const FRotator Rotation = GetControlRotation();
// 	const FVector Direction = UKismetMathLibrary::GetRightVector(Rotation);
// 		
// 	GetCharacter()->AddMovementInput(Direction, Value);
// }