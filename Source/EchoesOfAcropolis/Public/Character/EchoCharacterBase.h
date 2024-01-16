// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "NinjaCharacter.h"
#include "Interface/CombatInterface.h"
#include "EchoCharacterBase.generated.h"

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

	virtual FVector GetCombatAimLocation() override;

	virtual FVector GetCombatAimDirection() override;

protected:
	virtual void InitAbilityActorInfo() {}

	void AddCharacterAbilities();
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UAttributeSet* AttributeSet;

private:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
};
