// Copyright Echo of Acropolis. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EchoAbilitySet.generated.h"

class UGameplayEffect;
class UEchoGameplayAbility;
class UEchoAbilitySystemComponent;

/**
 * FEchoAbilitySet_GameplayAbility
 *
 *	Data used by the ability set to grant gameplay abilities.
 */
USTRUCT(BlueprintType)
struct FEchoAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEchoGameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * FEchoAbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FEchoAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * FEchoAbilitySet_AttributeSet
 *
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FEchoAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

};

/**
 * FEchoAbilitySet_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FEchoAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UEchoAbilitySystemComponent* EchoASC);

protected:

	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<UAttributeSet*> GrantedAttributeSets;
};

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API UEchoAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEchoAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	void GiveToAbilitySystem(class UEchoAbilitySystemComponent* EchoASC, FEchoAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObj = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability", meta = (TitleProperty = Ability))
	TArray<FEchoAbilitySet_GameplayAbility> AbilitiesToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect", meta = (TitleProperty = GameplayEffect))
	TArray<FEchoAbilitySet_GameplayEffect> EffectsToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Set", meta = (TitleProperty = AttributeSet))
	TArray<FEchoAbilitySet_AttributeSet> AttributeSetsToGrant;
};
