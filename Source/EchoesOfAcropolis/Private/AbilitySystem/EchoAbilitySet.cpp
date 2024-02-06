// Copyright Echo of Acropolis. All Rights Reserved.


#include "AbilitySystem/EchoAbilitySet.h"

#include "AbilitySystem/EchoAbilitySystemComponent.h"
#include "AbilitySystem/Ability/EchoGameplayAbility.h"

void FEchoAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FEchoAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FEchoAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FEchoAbilitySet_GrantedHandles::TakeFromAbilitySystem(UEchoAbilitySystemComponent* EchoASC)
{
	check(EchoASC);

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			EchoASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			EchoASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		TArray<UAttributeSet*>& AttributeSets = EchoASC->GetSpawnedAttributes_Mutable();
		AttributeSets.RemoveSingle(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UEchoAbilitySet::UEchoAbilitySet(const FObjectInitializer& ObjectInitializer)
{
}

void UEchoAbilitySet::GiveToAbilitySystem(UEchoAbilitySystemComponent* EchoASC,
	FEchoAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObj) const
{
	check(EchoASC);

	// Grant the gameplay abilities
	for (int32 AbilityIndex = 0; AbilityIndex < AbilitiesToGrant.Num(); ++AbilityIndex)
	{
		const FEchoAbilitySet_GameplayAbility& AbilityToGrant = AbilitiesToGrant[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}
		
		UEchoGameplayAbility* EchoAbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UEchoGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(EchoAbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObj;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = EchoASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects
	for (int32 EffectIndex = 0; EffectIndex < EffectsToGrant.Num(); ++EffectIndex)
	{
		const FEchoAbilitySet_GameplayEffect& EffectToGrant = EffectsToGrant[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid."), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = Cast<UGameplayEffect>(EffectToGrant.GameplayEffect.GetDefaultObject());
		const FActiveGameplayEffectHandle GameplayEffectHandle = EchoASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, EchoASC->MakeEffectContext());

		
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets
	for (int32 SetIndex = 0; SetIndex < AttributeSetsToGrant.Num(); ++SetIndex)
	{
		const FEchoAbilitySet_AttributeSet& SetToGrant = AttributeSetsToGrant[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedAttributeSets[%d] on ability set [%s] is not valid."), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* AttributeSet = NewObject<UAttributeSet>(EchoASC->GetOuter(), SetToGrant.AttributeSet);
		EchoASC->AddAttributeSetSubobject(AttributeSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(AttributeSet);
		}
	}
}

