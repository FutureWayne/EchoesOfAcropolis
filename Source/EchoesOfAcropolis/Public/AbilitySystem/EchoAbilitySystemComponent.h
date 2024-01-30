// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EchoAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityActorInfoSet();

	void AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& StartupAbilities);

	void AbilityTagHeld(const FGameplayTag& InputTag);
	void AbilityTagPressed(const FGameplayTag& InputTag);
	void AbilityTagReleased(const FGameplayTag& InputTag);

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	
};
