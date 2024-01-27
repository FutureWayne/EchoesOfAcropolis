// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "NinjaCharacter.h"
#include "Interface/CombatInterface.h"
#include "EchoCharacterBase.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class ECHOESOFACROPOLIS_API AEchoCharacterBase : public ANinjaCharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	explicit AEchoCharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/* Begin Combat Interface */
	
	virtual FVector GetCombatAimLocation() override;

	virtual FVector GetCombatAimDirection() override;

	virtual FVector GetWeaponTargetingSourceLocation(int WeaponIndex = 0) override;

	/* End Combat Interface */

protected:
	virtual void InitAbilityActorInfo() {}

	void InitDefaultAttributes() const;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;

	void AddCharacterAbilities() const;

	virtual void Die() override;
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Echo|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

private:
	UPROPERTY(EditAnywhere, Category = "Echo|Ability")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
};
