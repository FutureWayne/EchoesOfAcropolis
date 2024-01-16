// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/EchoInputConfig.h"

const UInputAction* UEchoInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FEchoInputAction& AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag.MatchesTag(InputTag))
		{
			return AbilityInputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEchoInputConfig::FindAbilityInputActionByTag: InputTag %s not found"), *InputTag.ToString());
	}

	return nullptr;
}
