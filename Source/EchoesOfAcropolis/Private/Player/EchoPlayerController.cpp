// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EchoPlayerController.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEchoPlayerController::AEchoPlayerController()
{
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

void AEchoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Jump", IE_Pressed, this, &AEchoPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AEchoPlayerController::StopJumping);
	
	InputComponent->BindAxis("MoveForward", this, &AEchoPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AEchoPlayerController::MoveRight);
	
	InputComponent->BindAxis("Turn", this, &AEchoPlayerController::AddYawInput);
	InputComponent->BindAxis("TurnRate", this, &AEchoPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &AEchoPlayerController::AddPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AEchoPlayerController::LookUpAtRate);
}

void AEchoPlayerController::Jump()
{
	GetCharacter()->Jump();
}

void AEchoPlayerController::StopJumping()
{
	GetCharacter()->StopJumping();
}

void AEchoPlayerController::MoveForward(float Value)
{
	if (Value == 0.0f)
	{
		return;
	}
	
	const FVector RightVector = UKismetMathLibrary::GetRightVector(GetControlRotation());
	const FVector UpVector = UKismetMathLibrary::GetUpVector(GetCharacter()->GetCapsuleComponent()->GetComponentRotation());

	const FVector Direction = FVector::CrossProduct(RightVector, UpVector);
	const FVector NormalizedDirection = UKismetMathLibrary::Normal(Direction);
		
	GetCharacter()->AddMovementInput(NormalizedDirection, Value);
}

void AEchoPlayerController::MoveRight(float Value)
{
	if (Value == 0.0f)
	{
		return;
	}
	
	const FRotator Rotation = GetControlRotation();
	const FVector Direction = UKismetMathLibrary::GetRightVector(Rotation);
		
	GetCharacter()->AddMovementInput(Direction, Value);
}

void AEchoPlayerController::TurnAtRate(const float Rate)
{
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AEchoPlayerController::LookUpAtRate(const float Rate)
{
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}