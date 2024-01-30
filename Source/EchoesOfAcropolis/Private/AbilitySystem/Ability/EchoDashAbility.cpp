// Copyright Echo of Acropolis. All Rights Reserved.


#include "AbilitySystem/Ability/EchoDashAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Camera/CameraComponent.h"
#include "Character/EchoCharacterBase.h"
#include "Character/EchoPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UEchoDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AEchoPlayer* Character = Cast<AEchoPlayer>(GetAvatarActorFromActorInfo());

	// Get the direction the character is facing
	const FVector FacingDirection = Character->GetActorForwardVector();

	// Get the last movement input
	const FVector LastMovementDirection = Character->GetLastMovementInputVector();
	// Cancel the ability if the character is not moving
	if (UKismetMathLibrary::Vector_IsNearlyZero(LastMovementDirection))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}
	
	// Get the aiming movement direction
	const FVector OriginalAimingDirection = Character->GetFollowCamera()->GetForwardVector();
	const FVector AimingDirection = OriginalAimingDirection * FVector(1.0f, 1.0f, 1.2f); // Add a little bit of height to the movement

	// If the character is dashing forward, use the aiming as dashing direction, otherwise use the movement input as direction
	const EDashDirection DashDirection = FindDashDirection(AimingDirection, LastMovementDirection);

	// TODO: Select dashing animation based on the dash direction
	
	if (DashDirection == EDashDirection::Forward)
	{
		Direction = AimingDirection;
	}
	else
	{
		Direction = LastMovementDirection;
	}

	// Cancel the ability if the dash movement direction is too small
	if (UKismetMathLibrary::Vector_IsNearlyZero(Direction))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	const bool bRet = CommitAbility(Handle, ActorInfo, ActivationInfo);
	if (!bRet)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	// TODO: Uncrouch the character if it is crouching

	// TODO: Play montage and wait

	// Apply root motion force
	UAbilityTask_ApplyRootMotionConstantForce* DashForceTask = ApplyDashForce();
	if (DashForceTask != nullptr)
	{
		DashForceTask->ReadyForActivation();
	}

	PlayDashEffect();
}

UAbilityTask_PlayMontageAndWait* UEchoDashAbility::PlayDashMontage()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	   this, 
	   NAME_None, 
	   DashAnimMontage, 
	   1.0f, 
	   NAME_None
   );

	if (Task != nullptr)
	{
		Task->OnInterrupted.AddDynamic(this, &UEchoDashAbility::OnDashCancelled);
		Task->OnCancelled.AddDynamic(this, &UEchoDashAbility::OnDashCancelled);
	}

	return Task;
}

UAbilityTask_ApplyRootMotionConstantForce* UEchoDashAbility::ApplyDashForce()
{
	UAbilityTask_ApplyRootMotionConstantForce* Task = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		NAME_None,
		Direction,
		Strength,
		RootMotionDuration,
		true,
		nullptr,
		ERootMotionFinishVelocityMode::SetVelocity,
		FVector(0.0f, 0.0f, 0.0f),
		1000.0f,
		false
	);

	if (Task != nullptr)
	{
		Task->OnFinish.AddUniqueDynamic(this, &UEchoDashAbility::OnDashCompleted);
	}

	return Task;
}

EDashDirection UEchoDashAbility::FindDashDirection(const FVector& FacingDirection, const FVector& LastMovementDirection)
{
	const FRotator FacingRotator = UKismetMathLibrary::MakeRotFromX(FacingDirection);
	const FRotator LastMovementRotator = UKismetMathLibrary::MakeRotFromX(LastMovementDirection);
	const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(FacingRotator, LastMovementRotator);
	
	if (abs(DeltaRotator.Yaw) < 45.0f)
	{
		return EDashDirection::Forward;
	}
	
	if (abs(DeltaRotator.Yaw) > 135.0f)
	{
		return EDashDirection::Backward;
	}

	if (DeltaRotator.Yaw > 0.0f)
	{
		return EDashDirection::Right;
	}
	
	else
	{
		return EDashDirection::Left;
	}
}

void UEchoDashAbility::PlayDashEffect() const
{
	// TODO: Play Dash Effect through gameplay cues
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, Avatar->GetActorLocation(), UKismetMathLibrary::MakeRotFromX(Direction));
}

void UEchoDashAbility::OnDashCancelled()
{
	if (const AEchoCharacterBase* Character = Cast<AEchoCharacterBase>(GetAvatarActorFromActorInfo()))
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UEchoDashAbility::OnDashCompleted()
{
	// const AActor* Avatar = GetAvatarActorFromActorInfo();
	// const FVector Location = Avatar->GetActorLocation();
	// const FRotator Rotation = UKismetMathLibrary::MakeRotFromX(Avatar->GetActorForwardVector());
	// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DashEffect, Location, Rotation);
	
	// Add a small delay before ending the ability
	UKismetSystemLibrary::Delay(this, AbilityDuration - RootMotionDuration, FLatentActionInfo(0, 0, TEXT("OnDashCompleted"), this));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
