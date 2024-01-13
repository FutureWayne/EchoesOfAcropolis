// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EchoPlayer.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/EchoPlayerState.h"

AEchoPlayer::AEchoPlayer(const FObjectInitializer& ObjectInitializer) 
	: AEchoCharacterBase(ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(540.0f, 540.0f, 540.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AEchoPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AEchoPlayer::InitAbilityActorInfo()
{
	AEchoPlayerState* EchoPlayerState = GetPlayerState<AEchoPlayerState>();
	check(EchoPlayerState);

	AbilitySystemComponent = EchoPlayerState->GetAbilitySystemComponent();
	AttributeSet = EchoPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->InitAbilityActorInfo(EchoPlayerState, this);
	Cast<UEchoAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();
	
	// TODO: Initialize Default Attributes thought gameplay effect
}

void AEchoPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEchoPlayer::BeginPlay()
{
	Super::BeginPlay();
}
