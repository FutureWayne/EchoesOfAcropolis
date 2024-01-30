// Copyright Echo of Acropolis. All Rights Reserved.


#include "AbilitySystem/Ability/EchoADSAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/EchoPlayer.h"

void UEchoADSAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AEchoPlayer* EchoPlayer = Cast<AEchoPlayer>(ActorInfo->AvatarActor.Get());
	if (EchoPlayer)
	{
		EchoPlayer->SetAimingStatus(true);
	}

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &UEchoADSAbility::OnInputReleased);
	WaitInputReleaseTask->ReadyForActivation();
}

void UEchoADSAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	AEchoPlayer* EchoPlayer = Cast<AEchoPlayer>(ActorInfo->AvatarActor.Get());
	if (EchoPlayer)
	{
		EchoPlayer->SetAimingStatus(false);
	}
}

void UEchoADSAbility::OnInputReleased(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, GetCurrentActorInfo(), CurrentActivationInfo, true, false);
}
