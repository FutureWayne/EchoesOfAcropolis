// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/EchoAbilitySystemComponent.h"

#include "AbilitySystem/Ability/EchoGameplayAbility.h"

void UEchoAbilitySystemComponent::OnAbilityActorInfoSet()
{
	// called from AEchoCharacterBase::InitAbilityActorInfo()
}

void UEchoAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			const UEchoGameplayAbility* EchoAbility = Cast<UEchoGameplayAbility>(AbilitySpec.Ability);
			if (EchoAbility)
			{
				AbilitySpec.DynamicAbilityTags.AddTag(EchoAbility->StartupInputTag);
				GiveAbility(AbilitySpec);
			}
		}
	}
}
