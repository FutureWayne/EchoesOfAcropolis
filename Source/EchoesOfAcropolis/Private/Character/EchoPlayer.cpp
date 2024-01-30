// Copyright Echo of Acropolis. All Rights Reserved.


#include "Character/EchoPlayer.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/EchoPlayerState.h"
#include "Singleton/EchoGameplayTags.h"

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

	DefaultFOV = FollowCamera->FieldOfView;
	CurrentFOV = DefaultFOV;
}

void AEchoPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AEchoPlayer::SetAimingStatus(bool bNewAimingStatus)
{
	bIsAiming = bNewAimingStatus;
}

void AEchoPlayer::SetDashStatus(bool bNewDashingStatus)
{
	if (bNewDashingStatus)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
	
}

void AEchoPlayer::ResetDashCooldown()
{
	const FEchoGameplayTags GameplayTags = FEchoGameplayTags::Get();
	AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(GameplayTags.Cooldown_Dash));
	OnDashCooldownReset();
}

void AEchoPlayer::InitAbilityActorInfo()
{
	AEchoPlayerState* EchoPlayerState = GetPlayerState<AEchoPlayerState>();
	check(EchoPlayerState);

	AbilitySystemComponent = EchoPlayerState->GetAbilitySystemComponent();
	AttributeSet = EchoPlayerState->GetAttributeSet();
	
	AbilitySystemComponent->InitAbilityActorInfo(EchoPlayerState, this);
	Cast<UEchoAbilitySystemComponent>(AbilitySystemComponent)->OnAbilityActorInfoSet();

	AddCharacterAbilities();
	InitDefaultAttributes();
}

FVector AEchoPlayer::GetCombatAimLocation()
{
	const FVector CameraLocation = FollowCamera->GetComponentLocation();
	const FRotator CameraRotation = FollowCamera->GetComponentRotation();

	FVector Start = CameraLocation;

	//TODO: 10000 is an arbitrary distance, consider use a attribute to config this
	FVector End = CameraLocation + (CameraRotation.Vector() * 10000);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the player character in the line trace

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	))
	{
		return HitResult.ImpactPoint;
	}
	return End;
}

FVector AEchoPlayer::GetCombatAimDirection()
{
	return FollowCamera->GetForwardVector();
}


void AEchoPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float TargetFOV = bIsAiming ? ZoomFOV : DefaultFOV;
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, ZoomSpeed);
	FollowCamera->SetFieldOfView(CurrentFOV);
}

void AEchoPlayer::BeginPlay()
{
	Super::BeginPlay();
}
