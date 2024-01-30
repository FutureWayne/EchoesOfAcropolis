// Copyright Echo of Acropolis. All Rights Reserved.


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

			// Check if this ability is already added
			bool bIsAbilityAlreadyAdded = false;
			for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
			{
				if (Spec.Ability == AbilitySpec.Ability)
				{
					bIsAbilityAlreadyAdded = true;
					break;
				}
			}

			if (bIsAbilityAlreadyAdded)
			{
				continue;
			}
			
			if (EchoAbility)
			{
				AbilitySpec.DynamicAbilityTags.AddTag(EchoAbility->StartupInputTag);
				FGameplayAbilitySpecHandle Handle = GiveAbility(AbilitySpec);
			}
		}
	}
}

void UEchoAbilitySystemComponent::AbilityTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	// debug print numbers of activatable abilities
	UE_LOG(LogTemp, Warning, TEXT("Number of activatable abilities: %d"), GetActivatableAbilities().Num());

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// Debug print Input Tag and whether ability is active or not
		UE_LOG(LogTemp, Warning, TEXT("InputTag: %s, IsActive: %s"), *InputTag.ToString(), AbilitySpec.IsActive() ? TEXT("true") : TEXT("false"));

		// Debug print all the activatable abilities from the ability system component
		TArray<FGameplayAbilitySpec>& AbilitySpecs = GetActivatableAbilities();
		
		
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UEchoAbilitySystemComponent::AbilityTagPressed(const FGameplayTag& InputTag)
{
}

void UEchoAbilitySystemComponent::AbilityTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UEchoAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UEchoAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}
